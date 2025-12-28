#include <simulate.hpp>

int main() {
    simulate sim;
    VirtualGraph g = build_virtual_graph(6,11);
    std::thread t(start_simulate, std::ref(sim), std::ref(g), 6, 11);

    while(!sim.initialized) 
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    int i = -1;
    while(true) {
        std::cout << "Введите цифру :\n"
                    "1 - поиск кратчайшего маршрута между двумя виртуальными узлами\n"
                    "2 - остановка программы" << std::endl;
        std::cin >> i;
        if(i == 1) {
            std::cout << "Введите две точки от 1 до 66" << std::endl;
            int a,b;
            std::cin >> a >> b;
            auto path = dijkstra(g, a, b);
            for(const auto& p : path.path) {
                try {
                    std::lock_guard<std::mutex> lg(GRID::grid_mtx);
                    auto ptr = GRID::grid_map.at(p);
                    auto satell = ptr.get_sid();
                    std::cout << satell.value() << std::endl;
                }
                catch (const std::exception& e) {
                    std::cout << "данный сервисный куб не обслуживается ни 1 спутником" << std::endl;
                }
            }        
        }
        if(i == 2) {
            sim.running = false;
            break;
        }
    }

    t.join();
    return 0;
}
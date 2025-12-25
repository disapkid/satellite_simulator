#include <graph.hpp>
#include <simulate.hpp>

int main() {
    simulate sim;
    std::thread t(start_simulate, std::ref(sim), 40);

    while(!sim.initialized) 
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

    int i = -1;
    while(i != 0) {
        std::cin >> i;
        if(i == 1) {
            for(int i = 0; i < 66; i++) {
                auto out = sim.get_pos(i);
                std::cout << out.x << " " << out.y << " " << out.z << std::endl;
            }
        }
        if(i == 2) {
            sim.running = false;
        }
    }

    t.join();
    return 0;
}
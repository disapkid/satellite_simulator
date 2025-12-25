#include <virtual_node.hpp>

void Virtual_node::change_sid(uint64_t sid) {
    s_id = sid;
}

Virtual_node::Virtual_node(uint64_t vid, double lat, double lon, double R) : v_id(vid), s_id(std::nullopt) {
    double clat = std::cos(lat);

    coordinates =  {
        R * clat * std::cos(lon),
        R * clat * std::sin(lon),
        R * std::sin(lat)
    };
}

// Разбиение поверхности Земли на сервисные ячейки и привязка V_Id к этим ячейкам
std::unordered_map<uint64_t, Virtual_node> buildEarthGrid(int M, int N) {
    std::unordered_map<uint64_t, Virtual_node> grid;
    static constexpr double R_EARTH = 6371000.0;
    grid.reserve(static_cast<size_t>(M) * N);

    for (int i = 0; i < M; ++i) {
        double lon = -M_PI + 2.0 * M_PI * (double(i) / M);

        for (int j = 0; j < N; ++j) {
            double lat = -M_PI / 2.0 + M_PI * (double(j) / (N - 1));

            uint64_t vid = static_cast<uint64_t>(i * N + j);
            grid.emplace(vid, Virtual_node(vid, lat, lon, R_EARTH));
        }
    }
    return grid;
}

std::unordered_map<uint64_t, Virtual_node> GRID::grid_map = buildEarthGrid(6,11);
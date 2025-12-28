#include <service_cube.hpp>

void Service_cube::change_sid(std::optional<uint64_t> sid) {
    s_id = sid;
}

std::optional<uint64_t> Service_cube::get_sid() const {
    if(s_id.has_value()) return s_id.value();
    return std::nullopt;
}

Service_cube::Service_cube(uint64_t vid, double lat, double lon, double R) : v_id(vid), s_id(std::nullopt) {
    double clat = std::cos(lat);

    coordinates =  {
        R * clat * std::cos(lon),
        R * clat * std::sin(lon),
        R * std::sin(lat)
    };
}

coord Service_cube::get_coord() const {
    return coordinates;
}

/* 
    Формирование grid+-сетки виртуальных узлов
    Проекция service cubes на поверхность Земли не является строгой тесселяцией
*/
std::unordered_map<uint64_t, Service_cube> buildEarthGrid(int M, int N) {
    std::unordered_map<uint64_t, Service_cube> grid;
    static constexpr double R_EARTH = 6371000.0;
    grid.reserve(static_cast<size_t>(M) * N);

    for (int i = 0; i < M; ++i) {
        double lon = -M_PI + 2.0 * M_PI * (double(i) / M);

        for (int j = 0; j < N; ++j) {
            double lat = -M_PI / 2.0 + M_PI * (double(j) / (N - 1));

            uint64_t vid = static_cast<uint64_t>(i * N + j) + 1;
            grid.emplace(vid, Service_cube(vid, lat, lon, R_EARTH));
        }
    }
    return grid;
}

// Фукнция проверки принадлежности координат спутника к сервисному кубу
bool Service_cube::Boolean(const coord& Loc, const Service_cube& C) {
    const auto center = C.get_coord();
    const double hs = C.half_size;

    bool result = 
                std::abs(Loc.x - center.x) <= hs &&
                std::abs(Loc.y - center.y) <= hs &&
                std::abs(Loc.z - center.z) <= hs;

    return result;
}

namespace GRID {
    std::unordered_map<uint64_t, Service_cube> grid_map = buildEarthGrid(6,11);
    std::mutex grid_mtx;
}
#pragma once
#include <satellite.hpp>
#include <unordered_map>
#include <optional>

class Service_cube final {
public:
    Service_cube(uint64_t vid, double lat, double lon, double R);

    void change_sid(std::optional<uint64_t> sid);

    std::optional<uint64_t> get_sid() const;

    coord get_coord() const;

    static bool Boolean(const coord& Loc, const Service_cube& C);
private:
    uint64_t v_id;
    std::optional<uint64_t> s_id;
    coord coordinates; 

    const double R = 6371e3;
    double half_size = 0.45 * M_PI * R / 6;
};

namespace GRID {
    // Получение сервисного куба по его V_ID   
    extern std::unordered_map<uint64_t, Service_cube> grid_map;
    extern std::mutex grid_mtx;
}
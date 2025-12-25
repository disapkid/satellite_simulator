#pragma once
#include <satellite.hpp>
#include <unordered_map>
#include <optional>

class Virtual_node final {
public:
    Virtual_node(uint64_t vid, double lat, double lon, double R);

    void change_sid(uint64_t sid);
private:
    uint64_t v_id;
    std::optional<uint64_t> s_id;
    coord coordinates;
};

namespace GRID {
    // Получение виртуального узла по V_Id узла    
    extern std::unordered_map<uint64_t, Virtual_node> grid_map;
}
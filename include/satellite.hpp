#pragma once
#include <cstdint>
#include <cmath>
#include <mutex>
#include <optional>

struct coord {
    double x, y, z;
};

struct spherical_coord {
    double r;  
    double lat;  
    double lon;  
};

struct spherical_vel {
    double dr;    
    double dlat;  
    double dlon;  
};

class Satellite final {
public:
    Satellite(double height_m = 0.0);

    void step(double dt);

    coord get_pos() const; 

    void set_coord_sph(const spherical_coord& r) { current_coord = r; }
    void set_velocity_sph(const spherical_vel& v) { velocity = v; }

private:
    spherical_vel accel(const spherical_coord& s, const spherical_vel& v) const;

    spherical_coord current_coord{};
    spherical_vel   velocity{};

    static constexpr double Re = 6371e3;
    static constexpr double mu = 3.986004415e14;
};

struct Satellite_node {
    void find_Vid();

    Satellite sat;
    uint64_t s_id{};
    std::optional<uint64_t> v_id = std::nullopt;
};
#pragma once
#include <cstdint>
#include <cmath>

struct coord {
    double x;
    double y;
    double z;
};

class Satellite final {
public:     
    Satellite(double height_m = 0.0) {
        current_coord = {Re + height_m, 0.0, 0.0};

        const double r = norm(current_coord);
        velocity = {0.0, std::sqrt(mu / r), 0.0};
    }

    void step(double dt);

    void set_coord(const coord& r);

    void set_velocity(const coord& v);

    coord get_pos() const;

    void 

private:
    double norm(const coord& r) const;

    coord accel(const coord& r) const;

    coord accel_j(const coord& r) const;

    coord current_coord{};
    coord velocity{};
     
    static constexpr double Re = 6371e3;    
    static constexpr double mu = 3.986004415e14;  
};


struct Satellite_node{
    Satellite sat;
    uint64_t s_id;
    uint64_t v_id;
};

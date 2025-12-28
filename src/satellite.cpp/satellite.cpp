#include "satellite.hpp"
#include <service_cube.hpp>

Satellite::Satellite(double height_m) {
    current_coord.r   = Re + height_m;
    current_coord.lat = 0.0;
    current_coord.lon = 0.0;

    velocity.dr   = 0.0;
    velocity.dlat = 0.0;
    velocity.dlon = std::sqrt(mu / current_coord.r) / current_coord.r; 
}

coord Satellite::get_pos() const {
    const double cl = std::cos(current_coord.lat);
    
    return {
        current_coord.r * cl * std::cos(current_coord.lon),
        current_coord.r * cl * std::sin(current_coord.lon),
        current_coord.r *      std::sin(current_coord.lat)
    };
}

// Вычисление ускорения
spherical_vel Satellite::accel(const spherical_coord& s,
                               const spherical_vel& v) const {
    spherical_vel a{};

    const double sin_lat = std::sin(s.lat);
    const double cos_lat = std::cos(s.lat);

    a.dr =
        s.r * (v.dlat * v.dlat +
               v.dlon * v.dlon * cos_lat * cos_lat)
        - mu / (s.r * s.r);

    a.dlat =
        -2.0 * v.dr * v.dlat / s.r
        + v.dlon * v.dlon * sin_lat * cos_lat;

    a.dlon =
        -2.0 * v.dr * v.dlon / s.r
        -2.0 * v.dlat * v.dlon * std::tan(s.lat);

    return a;
}

// Вычисление скорости и положения с помощью метода Рунге-Кутты 4 порядка
void Satellite::step(double dt) {
    auto k1_r = velocity;
    auto k1_v = accel(current_coord, velocity);

    spherical_coord r2 {
        current_coord.r   + 0.5 * dt * k1_r.dr,
        current_coord.lat + 0.5 * dt * k1_r.dlat,
        current_coord.lon + 0.5 * dt * k1_r.dlon
    };
    spherical_vel v2 {
        velocity.dr   + 0.5 * dt * k1_v.dr,
        velocity.dlat + 0.5 * dt * k1_v.dlat,
        velocity.dlon + 0.5 * dt * k1_v.dlon
    };

    auto k2_r = v2;
    auto k2_v = accel(r2, v2);

    spherical_coord r3 {
        current_coord.r   + 0.5 * dt * k2_r.dr,
        current_coord.lat + 0.5 * dt * k2_r.dlat,
        current_coord.lon + 0.5 * dt * k2_r.dlon
    };
    spherical_vel v3 {
        velocity.dr   + 0.5 * dt * k2_v.dr,
        velocity.dlat + 0.5 * dt * k2_v.dlat,
        velocity.dlon + 0.5 * dt * k2_v.dlon
    };

    auto k3_r = v3;
    auto k3_v = accel(r3, v3);

    spherical_coord r4 {
        current_coord.r   + dt * k3_r.dr,
        current_coord.lat + dt * k3_r.dlat,
        current_coord.lon + dt * k3_r.dlon
    };
    spherical_vel v4 {
        velocity.dr   + dt * k3_v.dr,
        velocity.dlat + dt * k3_v.dlat,
        velocity.dlon + dt * k3_v.dlon
    };

    auto k4_r = v4;
    auto k4_v = accel(r4, v4);

    current_coord.r   += dt / 6.0 * (k1_r.dr   + 2*k2_r.dr   + 2*k3_r.dr   + k4_r.dr);
    current_coord.lat += dt / 6.0 * (k1_r.dlat + 2*k2_r.dlat + 2*k3_r.dlat + k4_r.dlat);
    current_coord.lon += dt / 6.0 * (k1_r.dlon + 2*k2_r.dlon + 2*k3_r.dlon + k4_r.dlon);

    velocity.dr   += dt / 6.0 * (k1_v.dr   + 2*k2_v.dr   + 2*k3_v.dr   + k4_v.dr);
    velocity.dlat += dt / 6.0 * (k1_v.dlat + 2*k2_v.dlat + 2*k3_v.dlat + k4_v.dlat);
    velocity.dlon += dt / 6.0 * (k1_v.dlon + 2*k2_v.dlon + 2*k3_v.dlon + k4_v.dlon);
}

void Satellite_node::find_Vid() {
    auto Loc = sat.get_pos();
    std::lock_guard<std::mutex> lg(GRID::grid_mtx);

    if(v_id.has_value()) {
        auto ptr = GRID::grid_map.find(v_id.value());
        if(ptr != GRID::grid_map.end()) {
            ptr->second.change_sid(std::nullopt);
        }
    }

    v_id.reset();

    for(auto& [id, node] : GRID::grid_map) {

        if(Service_cube::Boolean(Loc, node)) {
            v_id = id;
            node.change_sid(s_id);
            return;
        }
    }
}
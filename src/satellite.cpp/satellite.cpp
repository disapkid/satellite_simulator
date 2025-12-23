#include <satellite.hpp>

inline double Satellite::norm(const coord& r) const{
    auto phi = r.x * r.x + r.y * r.y + r.z * r.z;
    return std::sqrt(phi);
}

coord Satellite::get_pos() const {
    return current_coord;
}

void Satellite::step(double dt) {
    coord k1_r = Satellite::velocity;
    coord k1_v = Satellite::accel(current_coord);

    coord r2 = {
        current_coord.x + 0.5 * dt * k1_r.x,
        current_coord.y + 0.5 * dt * k1_r.y,
        current_coord.z + 0.5 * dt * k1_r.z,
    };

    coord v2 = {
        velocity.x + 0.5 * dt * k1_v.x,
        velocity.y + 0.5 * dt * k1_v.y,
        velocity.z + 0.5 * dt * k1_v.z
    };

    coord k2_r = v2;
    coord k2_v = Satellite::accel(r2);

    coord r3 = {
        current_coord.x + 0.5 * dt * k2_r.x,
        current_coord.y + 0.5 * dt * k2_r.y,
        current_coord.z + 0.5 * dt * k2_r.z,
    };
    coord v3 = {
        velocity.x + 0.5 * dt * k2_v.x,
        velocity.y + 0.5 * dt * k2_v.y,
        velocity.z + 0.5 * dt * k2_v.z
    };

    coord k3_r = v3;
    coord k3_v = Satellite::accel(r3);

    coord r4 = {
        current_coord.x + dt * k3_r.x,
        current_coord.y + dt * k3_r.y,
        current_coord.z + dt * k3_r.z,
    };
    coord v4 = {
        velocity.x + dt * k3_v.x,
        velocity.y + dt * k3_v.y,
        velocity.z + dt * k3_v.z
    };

    coord k4_r = v4;
    coord k4_v = Satellite::accel(r4);

    current_coord.x += dt / 6 * (k1_r.x + 2*k2_r.x + 2*k3_r.x + k4_r.x);
    current_coord.y += dt / 6 * (k1_r.y + 2*k2_r.y + 2*k3_r.y + k4_r.y);
    current_coord.z += dt / 6 * (k1_r.z + 2*k2_r.z + 2*k3_r.z + k4_r.z);

    velocity.x += dt / 6 * (k1_v.x + 2*k2_v.x + 2*k3_v.x + k4_v.x);
    velocity.y += dt / 6 * (k1_v.y + 2*k2_v.y + 2*k3_v.y + k4_v.y);
    velocity.z += dt / 6 * (k1_v.z + 2*k2_v.z + 2*k3_v.z + k4_v.z);
}

inline coord Satellite::accel(const coord& r) const{
    auto r_k = Satellite::norm(r);
    auto r3 = pow(r_k,3);

    auto x_k = -mu*(r.x / r3);
    auto x_y = -mu*(r.y / r3);
    auto x_z = -mu*(r.z / r3);

    auto j_2 = Satellite::accel_j(r);
    x_k += j_2.x;
    x_y += j_2.y;
    x_z += j_2.z;

    return coord(x_k, x_y, x_z);
}

inline coord Satellite::accel_j(const coord& r) const{
    coord a_j;
    auto r_k = Satellite::norm(r);
    constexpr double j2 = 1.08262668e-3;

    auto kf = (3*j2*mu*Re*Re) / (2 * pow(r_k,5));

    auto z2 = r.z * r.z;
    auto r2 = r_k * r_k;

    a_j.x = kf * (5 * z2 / r2 - 1) * r.x;
    a_j.y = kf * (5 * z2 / r2 - 1) * r.y;
    a_j.z = kf * (5 * z2 / r2 - 3) * r.z;

    return a_j;
}

void Satellite::set_coord(const coord& r) {
    current_coord.x = r.x;
    current_coord.y = r.y;
    current_coord.z = r.z;
}

void Satellite::set_velocity(const coord& v) {
    velocity.x = v.x;
    velocity.y = v.y;
    velocity.z = v.z;
}
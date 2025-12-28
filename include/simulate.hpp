#pragma once
#include <satellite.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <graph_search.hpp>

struct simulate
{
    std::vector<Satellite_node> node;
    double dt = 0.05;
    double simulation_t = 0;

    std::mutex mutex;
    std::atomic<bool> initialized{false};
    std::atomic<bool> running{true};

    void step() {
        std::for_each(node.begin(), node.end(), [&] (Satellite_node& sn) {
            sn.sat.step(dt);
            sn.find_Vid();
        });

        simulation_t += dt;
    }

    coord get_pos(int i) {
        std::lock_guard<std::mutex> mtx(mutex);
        return node[i].sat.get_pos();
    }
};

// Создание спутников на разных стартовых позициях
std::vector<Satellite_node> Init_sat_nodes(int M, int N) {
    std::vector<Satellite_node> vec;
    vec.reserve(M * N);

    constexpr double h  = 400e3;
    constexpr double Re = 6371e3;
    constexpr double mu = 3.986004418e14;

    constexpr double R = Re + h;
    const double omega = std::sqrt(mu / (R * R * R));

    const double inclination = M_PI / 2.0; 

    uint64_t sid = 1;

    for (int p = 0; p < M; ++p) {
        double raan = 2.0 * M_PI * p / M;

        for (int k = 0; k < N; ++k) {
            double phase = 2.0 * M_PI * k / N;

            Satellite_node sn;
            sn.s_id = sid++;
            sn.v_id = std::nullopt;

            sn.sat = Satellite(h);

            sn.sat.set_coord_sph({
                R,
                inclination * std::sin(phase),
                raan
            });

            sn.sat.set_velocity_sph({
                0.0,
                omega,
                0.0
            });

            vec.push_back(std::move(sn));
        }
    }

    return vec;
}

void start_simulate(simulate& sim, VirtualGraph& g, int M, int N) {
    using namespace std::chrono;

    {
        std::lock_guard<std::mutex> lg(sim.mutex);
        sim.node = Init_sat_nodes(M, N);
        sim.initialized = true;
    }

    auto time_start = steady_clock::now();

    while (sim.running) {
        auto now = steady_clock::now();
        double t_passed = duration<double>(now - time_start).count();

        std::lock_guard<std::mutex> lg(sim.mutex);
        while (sim.dt + sim.simulation_t <= t_passed) {
            sim.step();
        }

        std::this_thread::sleep_for(milliseconds(1));
    }
}

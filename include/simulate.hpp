#pragma once
#include <satellite.hpp>
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>

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
        });

        simulation_t += dt;
    }

    coord get_pos(int i) {
        std::lock_guard<std::mutex> mtx(mutex);
        return node[i].sat.get_pos();
    }
};

std::vector<Satellite_node> init_nodes(int N) {
    std::vector<Satellite_node> vec;

    constexpr double h = 400e3;
    constexpr double Re = 6371e3;
    constexpr double mu = 3.986004418e14;

    constexpr double R  = Re + h;
    const double v0 = std::sqrt(mu / R);

    for (int i = 0; i < N; ++i) {
        double phase = 2.0 * M_PI * i / N;

        Satellite_node sn;
        sn.s_id = i + 1;
        sn.v_id = 0;

        sn.sat = Satellite(h); 
        sn.sat.set_coord({
            R * std::cos(phase),
            R * std::sin(phase),
            0.0
        });

        sn.sat.set_velocity({
            -v0 * std::sin(phase),
            v0 * std::cos(phase),
            0.0
        });

        vec.push_back(sn);
    }

    return vec;
}

void start_simulate(simulate& sim, int N) {
    using namespace std::chrono;

    {
        std::lock_guard<std::mutex> lg(sim.mutex);
        sim.node = init_nodes(N);
        sim.initialized = true;
    }

    auto time_start = steady_clock::now();

    while (sim.running)
    {
        auto now = steady_clock::now();

        double t_passed = duration<double>(now - time_start).count();

        std::lock_guard<std::mutex> mtx(sim.mutex);
        while(sim.dt + sim.simulation_t <= t_passed) {
            sim.step();
        }

        std::this_thread::sleep_for(milliseconds(1));
    }
}
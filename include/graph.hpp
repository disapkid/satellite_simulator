#pragma once
#include <virtual_node.hpp>
#include <satellite.hpp>
#include <vector>

struct Edge {
    uint64_t to;     
    double weight;
};

struct VirtualGraph {
    std::unordered_map<uint64_t, Virtual_node> nodes;
    std::unordered_map<uint64_t, std::vector<Edge>> adj;
};

// Построение графа сети 
VirtualGraph buildGrid() {
    constexpr int M = 6;
    constexpr int N = 11;
    constexpr double R_EARTH = 6371000.0;

    VirtualGraph g;

    inline auto vid = [&](int p, int k) {
        return static_cast<uint64_t>(p * N + k);
    };

    for (int p = 0; p < M; ++p) {
        double lon = -M_PI + 2.0 * M_PI * (double(p) / M);

        for (int k = 0; k < N; ++k) {
            double lat = -M_PI / 2.0 + M_PI * (double(k) / (N - 1));
            uint64_t id = vid(p, k);

            g.nodes.try_emplace(id, id, lat, lon, R_EARTH);
        }
    }

    for (int p = 0; p < M; ++p) {
        for (int k = 0; k < N; ++k) {
            uint64_t v = vid(p, k);

            auto add_edge = [&](uint64_t to) {
                g.adj[v].push_back({to, 1.0});
            };

            add_edge(vid(p, (k + 1) % N));
            add_edge(vid(p, (k - 1 + N) % N));

            int p_next = (p + 1) % M;
            int p_prev = (p - 1 + M) % M;

            if (!(p == 5 && p_next == 0))
                add_edge(vid(p_next, k));

            if (!(p == 0 && p_prev == 5))
                add_edge(vid(p_prev, k));
        }
    }

    return g;
}
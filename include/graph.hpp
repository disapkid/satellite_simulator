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

double distance(const Virtual_node& a, const Virtual_node& b) {
    double dx = a.get_coord().x - b.get_coord().x;
    double dy = a.get_coord().y - b.get_coord().y;
    double dz = a.get_coord().z - b.get_coord().z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

VirtualGraph buildGrid() {
    constexpr int M = 6;
    constexpr int N = 11;
    constexpr double R_EARTH = 6371000.0;

    VirtualGraph g;

    auto vid = [&](int p, int k) -> uint64_t {
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

            auto add_edge = [&](uint64_t from, uint64_t to) {
                const auto& A = g.nodes.at(from);
                const auto& B = g.nodes.at(to);
                double w = distance(A, B);
                g.adj[from].push_back({to, w});
            };


            if (k + 1 < N) {
                add_edge(v, vid(p, k + 1));
                add_edge(vid(p, k + 1), v);
            }
            if (k > 0) {
                add_edge(v, vid(p, k - 1));
                add_edge(vid(p, k - 1), v);
            }

            int p_next = p + 1;
            int p_prev = p - 1;

            if (p_next < M && !(p == 5 && p_next == 0)) {
                add_edge(v, vid(p_next, k));
                add_edge(vid(p_next, k), v);
            }

            if (p_prev >= 0 && !(p == 0 && p_prev == 5)) {
                add_edge(v, vid(p_prev, k));
                add_edge(vid(p_prev, k), v);
            }
        }
    }

    return g;
}
#pragma once
#include <service_cube.hpp>
#include <vector>

struct Edge {
    uint64_t to;
    double weight;
};

struct VirtualGraph {
    std::unordered_map<uint64_t, std::vector<Edge>> adj;
};

inline double dist3(const coord& a, const coord& b) {
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    const double dz = a.z - b.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

VirtualGraph build_virtual_graph(int M, int N) {
    VirtualGraph g;
    g.adj.reserve(GRID::grid_map.size());

    auto vid = [N](int p, int k) -> uint64_t {
        return static_cast<uint64_t>(p) * N + static_cast<uint64_t>(k) + 1;
    };

    auto add_edge = [&](uint64_t a, uint64_t b) {
        const coord& ca = GRID::grid_map.at(a).get_coord();
        const coord& cb = GRID::grid_map.at(b).get_coord();
        const double w = dist3(ca, cb);

        g.adj[a].push_back({b, w});
        g.adj[b].push_back({a, w});
    };

    // intraplane (по широте)
    for (int p = 0; p < M; ++p) {
        for (int k = 0; k + 1 < N; ++k) {
            add_edge(vid(p, k), vid(p, k + 1));
        }
    }

    // interplane (по долготе, с замыканием)
    for (int p = 0; p < M; ++p) {
        int p_next = (p + 1) % M;
        for (int k = 0; k < N; ++k) {
            add_edge(vid(p, k), vid(p_next, k));
        }
    }

    return g;
}

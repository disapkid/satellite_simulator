#include <graph.hpp>
#include <queue>
#include <algorithm>
#include <limits>

struct DijkstraResult {
    std::unordered_map<uint64_t, double> dist;
    std::unordered_map<uint64_t, uint64_t> prev;
};

DijkstraResult dijkstra(const VirtualGraph& g, uint64_t source) {
    constexpr double INF = std::numeric_limits<double>::infinity();
    DijkstraResult res;

    for (const auto& [vid, _] : g.nodes) {
        res.dist[vid] = INF;
    }
    res.dist[source] = 0.0;

    using QNode = std::pair<double, uint64_t>; 
    std::priority_queue<QNode, std::vector<QNode>, std::greater<>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (d > res.dist[v])
            continue;

        if (!g.adj.contains(v))
            continue;

        for (const auto& e : g.adj.at(v)) {
            double nd = d + e.weight;

            if (nd < res.dist[e.to]) {
                res.dist[e.to] = nd;
                res.prev[e.to] = v;
                pq.push({nd, e.to});
            }
        }
    }
    return res;
}

// Вывод пути от узла А до узла В
std::vector<uint64_t> restore_path(uint64_t target,
                                   const std::unordered_map<uint64_t, uint64_t>& prev) {
    std::vector<uint64_t> path;

    uint64_t cur = target;
    path.push_back(cur);

    while (prev.contains(cur)) {
        cur = prev.at(cur);
        path.push_back(cur);
    }

    std::reverse(path.begin(), path.end());

    return path;
}
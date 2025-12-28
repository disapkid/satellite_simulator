#pragma once
#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>
#include <graph.hpp>

struct PathResult {
    double distance;              
    std::vector<uint64_t> path;      
};

PathResult dijkstra (
    const VirtualGraph& g,
    uint64_t start,
    uint64_t goal ) 
{
    constexpr double INF = std::numeric_limits<double>::infinity();

    std::unordered_map<uint64_t, double> dist;
    std::unordered_map<uint64_t, uint64_t> prev;

    for (const auto& [v, _] : g.adj) {
        dist[v] = INF;
    }
    dist[start] = 0.0;

    using QNode = std::pair<double, uint64_t>;
    std::priority_queue<QNode, std::vector<QNode>, std::greater<>> pq;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == goal) break;

        for (const Edge& e : g.adj.at(u)) {
            double nd = d + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                prev[e.to] = u;
                pq.push({nd, e.to});
            }
        }
    }

    PathResult res;
    res.distance = dist[goal];

    if (res.distance == INF) {
        return res;
    }

    for (uint64_t v = goal; v != start; v = prev[v]) {
        res.path.push_back(v);
    }
    res.path.push_back(start);
    std::reverse(res.path.begin(), res.path.end());

    return res;
}
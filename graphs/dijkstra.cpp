#include <cstdlib>
#include <queue>
#include <cassert>
#include <algorithm>
#include <cstring>

template<typename EdgeWeight>
struct WeightedDirectedGraph {
    struct Edge {
        int neighbor;
        EdgeWeight weight;
    };

    int N;
    std::vector<std::vector<Edge>> adj;

    WeightedDirectedGraph (int _N) : N(_N), adj(_N) {}

    void add_directed_edge(int u, int v, EdgeWeight w) {
        adj[u].push_back({v, w});
    }
};

template<typename PathWeight>
struct ShortestPathTree {
    std::vector<int> parent;
    std::vector<PathWeight> shortest_path_wt;

    template<typename EdgeWeight, typename JoinPathAndEdge, typename PathWeightLessThan>
    ShortestPathTree(WeightedDirectedGraph<EdgeWeight> g, const std::vector<int> &sources,
            PathWeight init, JoinPathAndEdge join, PathWeightLessThan less_than) {
        struct Path {
            int destination;
            PathWeight weight;
        };

        auto path_cmp = [&less_than](const Path &a, const Path &b) {
            return less_than(b.weight, a.weight);
        };

        std::priority_queue<Path, std::vector<Path>, decltype(path_cmp)> pq(path_cmp);

        parent.assign(g.N, -1);
        shortest_path_wt.assign(g.N, init);

        for (int source : sources) {
            parent[source] = source;
            pq.push({ source, shortest_path_wt[source] });
        }

        while (!pq.empty()) {
            Path path = pq.top();
            pq.pop();

            if (memcmp(&path.weight, &shortest_path_wt[path.destination], sizeof(PathWeight)))
                continue;

            for (auto edge : g.adj[path.destination]) {
                PathWeight candidate = join(path.weight, edge.weight);
                if (parent[edge.neighbor] == -1 || less_than(candidate, shortest_path_wt[edge.neighbor])) {
                    parent[edge.neighbor] = path.destination;
                    shortest_path_wt[edge.neighbor] = candidate;
                    pq.push({ edge.neighbor, shortest_path_wt[edge.neighbor] });
                }
            }
        }
    }

    bool is_reachable(int destination) const {
        return parent[destination] != -1;
    }

    PathWeight distance(int destination) const {
        if (!is_reachable(destination)) assert(false);
        return shortest_path_wt[destination];
    }

    std::vector<int> list_vertices_on_path(int destination) const {
        if (!is_reachable(destination)) assert(false);

        std::vector<int> path;
        while (parent[destination] != destination) {
            path.push_back(destination);
            destination = parent[destination];
        }
        path.push_back(destination);

        std::reverse(path.begin(), path.end());
        return path;
    }
};

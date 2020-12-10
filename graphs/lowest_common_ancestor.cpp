// {{{ graphs/tree.cpp }}}
// {{{ data_structures/sparse_table.cpp }}}

#include <vector>

template<typename Edge>
struct lowest_common_ancestor {
    const tree<Edge>& t;

    std::vector<int> euler_tour, tour_depths, first_visit, last_visit;

    int operator()(int i, int j) const {
        // if two visits have equal depth we prefer the later, allowing us to compute first_step
        return tour_depths[i] < tour_depths[j] ? i : j;
    }

    sparse_table<int, const lowest_common_ancestor<Edge>&> tour_table;

    lowest_common_ancestor() {}

    lowest_common_ancestor(const tree<Edge>& _t) : t(_t), first_visit(t.V), last_visit(t.V), tour_table(*this) {
        record_tour(t.root);
        tour_table.construct(int(euler_tour.size()), [](int i) { return i; });
    }

    void record_tour(int u) {
        first_visit[u] = int(euler_tour.size());
        euler_tour.push_back(u);
        tour_depths.push_back(t.depth[u]);

        t.for_each_child(u, [&](const Edge& e) {
            record_tour(e.get_nbr(u));
            euler_tour.push_back(u);
            tour_depths.push_back(t.depth[u]);
        });

        last_visit[u] = int(euler_tour.size()) - 1;
    }

    int lca(int u, int v) const {
        u = first_visit[u], v = first_visit[v];
        if (u > v) std::swap(u, v);
        return euler_tour[tour_table.accumulate(u, v + 1)];
    }

    int dist(int u, int v) const {
        return t.depth[u] + t.depth[v] - 2 * t.depth[lca(u, v)];
    }

    bool uv_path_has_w(int u, int v, int w) const {
        return w != -1 && dist(u, v) == (dist(u, w) + dist(w, v));
    }

    bool is_ancestor(int anc, int desc) const {
        return first_visit[anc] <= first_visit[desc] && first_visit[desc] <= last_visit[anc];
    }

    // Returns the neighbor of u on the simple path from u to v
    int first_step(int u, int v) const {
        assert(u != v);
        if (!is_ancestor(u, v)) return t.parent[u];
        return euler_tour[tour_table.accumulate(first_visit[u], first_visit[v]) + 1];
    }
};

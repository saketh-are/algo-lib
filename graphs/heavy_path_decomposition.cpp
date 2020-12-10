// {{{ graphs/tree.cpp }}}

using index_t = unsigned int;
using interval_t = std::pair<index_t, index_t>;
using path_t = std::vector<interval_t>;

template<typename Edge>
struct heavy_path_decomposition {
    const tree<Edge> &t;

    struct node {
        index_t index;
        int heavy_path_top;
    };
    std::vector<node> data;

    heavy_path_decomposition() {}

    heavy_path_decomposition(const tree<Edge> &_t) : t(_t), data(t.V) {
        int current_heavy_path_top = -1;
        for (int i = 0; i < t.V; i++) {
            int u = t.preorder[i];

            if (i == 0 || t.preorder[i - 1] != t.parent[u])
                current_heavy_path_top = u;

            data[u] = { (unsigned int)i, current_heavy_path_top };
        }
    }

    index_t index(int v) const {
        return data[v].index;
    }

    int at_index(index_t i) const {
        return t.preorder[i];
    }

    int htop(int v) const {
        return data[v].heavy_path_top;
    }

    int lca(int u, int v) const {
        while (htop(u) != htop(v)) {
            int& jump = index(htop(u)) > index(htop(v)) ? u : v;
            jump = t.par[htop(jump)];
        }
        return t.depth[u] < t.depth[v] ? u : v;
    }

    int dist(int u, int v) const {
        return t.depth[u] + t.depth[v] - 2 * t.depth[lca(u, v)];
    }

    bool uv_path_has_w(int u, int v, int w) const {
        return w != -1 && dist(u, v) == (dist(u, w) + dist(w, v));
    }

    bool is_ancestor(int anc, int desc) const {
        return index(anc) <= index(desc) && index(desc) < index(anc) + t.subt_sz[anc];
    }

    Edge first_step(int u, int v) const {
        assert(u != v);
        if (!is_ancestor(u, v)) return t.up_edge(u);
        return *upper_bound(all(t.children[u]), v,
            [&](int _v, Edge c) { return index(v) < index(c(u)) + t.subt_sz[c(u)]; });
    }

    int kth_ancestor(int u, int k) const {
        assert(0 <= k && k <= t.depth[u]);
        while (true) {
            int dist_to_htop = t.depth[u] - t.depth[htop(u)];
            if (k <= dist_to_htop)
                return at_index(index(u) - k);
            u = t.par[htop(u)];
            k -= dist_to_htop + 1;
        }
    }

    int kth_step(int u, int v, int k) const {
        int w = lca(u, v), d = dist(u, v);
        assert(d >= k);
        return k <= t.depth[u] - t.depth[w] ? kth_ancestor(u, k) : kth_ancestor(v, d - k);
    }

    mutable int path_lca;
    mutable path_t path_up, path_down;
    void decompose_path(int u, int v, bool include_lca) const {
        path_up.clear(), path_down.clear();
        while (htop(u) != htop(v)) {
            int &jump = index(htop(u)) > index(htop(v)) ? u : v;
            (jump == u ? path_up : path_down).emplace_back(index(htop(jump)), index(jump) + 1);
            jump = t.parent[htop(jump)];
        }

        path_lca = t.depth[u] < t.depth[v] ? u : v;
        if (u != v || include_lca) {
            path_t &side = (u == path_lca ? path_down : path_up);
            side.emplace_back(index(path_lca) + !include_lca, index(path_lca ^ u ^ v) + 1);
        }

        reverse(path_down.begin(), path_down.end());
    }

    template<typename LeftFold, typename RightFold>
    void for_each(int u, int v, bool include_lca, LeftFold left_fold, RightFold right_fold) const {
        decompose_path(u, v, include_lca);
        for (interval_t r : path_up)    left_fold(r.first, r.second);
        for (interval_t r : path_down) right_fold(r.first, r.second);
    }

    template<typename LeftFold>
    void for_each_commutative(int u, int v, bool include_lca, LeftFold left_fold) const {
        for_each(u, v, include_lca, left_fold, left_fold);
    }

    template<typename T, typename AssociativeOperation, typename LeftFold, typename RightFold>
    T accumulate(int u, int v, bool include_lca, T init,
            AssociativeOperation op, LeftFold left_fold, RightFold right_fold) {
        decompose_path(u, v, include_lca);
        for (interval_t r : path_up)   init = op(init,  left_fold(r.first, r.second));
        for (interval_t r : path_down) init = op(init, right_fold(r.first, r.second));
        return init;
    }

    template<typename T, typename AssociativeOperation, typename LeftFold>
    T accumulate_commutative(int u, int v, bool include_lca, T init,
            AssociativeOperation op, LeftFold left_fold) {
        return accumulate(u, v, include_lca, init, op, left_fold, left_fold);
    }
};

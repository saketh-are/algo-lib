template<typename E> struct condensed_tree {
    const lowest_common_ancestor<E>& t;
    vi label, above; vb interesting; int root, L;

    condensed_tree(const lowest_common_ancestor<E>& _t, vi points_of_interest,
            int _root = -1) : t(_t), label(_t.t.V, -1), interesting(_t.t.V, 0), root(_root), L(0) {
        sort_by(points_of_interest, t.first_visit[a] < t.first_visit[b]);
        unique(all(points_of_interest));
        for (int i = 0; i < sz(points_of_interest); i++) {
            interesting[points_of_interest[i]] = true;
            if (i) interesting[t.lca(points_of_interest[i-1], points_of_interest[i])] = true;
        }
        if (root != -1) interesting[root] = true;

        for (int u : t.preorder) if (interesting[u]) {
            label[u] = L;
            int v = t.par(u);
            if (v == -1 || interesting[v]) {
                above.pb(v != -1 ? label[v] : -1), L++;
                continue;
            }
            if (root == -1) above.pb(++L);
            for (; v != -1 && !interesting[v]; v = t.par(v))
                label[v] = L;
            above.pb(v != -1 ? label[v] : -1), L++;
        }
    }

    const vi& decompose_path(int u, int v, bool include_lca) const {
        static vi res; res.clear();
        int w = t.lca(u, v); if (root != -1) assert(root == w);
        assert(interesting[u] && interesting[v] && interesting[w]);
        for (int x = label[u]; x != label[w]; x = above[x]) res.pb(x);
        if (include_lca) res.pb(label[w]);
        size_t bef = res.size();
        for (int x = label[v]; x != label[w]; x = above[x]) res.pb(x);
        reverse(res.begin() + bef, res.end());
        return res;
    }

    template<typename F>
    void for_each_label_commutative(int u, int v, bool include_lca, F f) const {
        int w = t.lca(u, v); if (root != -1) assert(root == w);
        assert(interesting[u] && interesting[v] && interesting[w]);
        for (int x = label[u]; x != label[w]; x = above[x]) f(x);
        if (include_lca) f(label[w]);
        for (int x = label[v]; x != label[w]; x = above[x]) f(x);
    }
};

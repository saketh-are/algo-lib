template<typename E> struct heavy_path_decomposition : lowest_common_ancestor<E> {
    struct heavy_path { int index, htop, hbot; };
    vector<heavy_path> hld;
    vi preorder;

    int par(int v) const { return this->_par[v]; }
    int depth(int v) const { return this->_depth[v]; }
    int subt_sz(int v) const { return this->_subt_sz[v]; }

    heavy_path_decomposition(tree<E> _t) : lowest_common_ancestor<E>(_t) {
        hld.resz(this->t.V), preorder.resz(this->t.V);
        auto dfs = [&](auto& self, int loc, int index, int htop) -> int {
            preorder[index] = loc;
            hld[loc] = {index++, htop, -1};
            auto it = max_element(all(this->nbrs(loc)), [&](int u, int v) {
                if (u == v) return 0;
                return u == par(loc) ? 1 : v == par(loc) ? 0 : subt_sz(u) < subt_sz(v);
            });
            if (*it != par(loc)) {
                index = self(self, *it, index, htop);
            } else {
                hld[htop].hbot = loc;
            }
            for (int nbr : this->nbrs(loc)) if (nbr != par(loc) && nbr != *it) {
                index = self(self, nbr, index, nbr);
            }
            return index;
        };
        dfs(dfs, 0, 0, 0);
    }

    int index(int v) const { return hld[v].index; }
    int at_index(int i) const { return preorder[i]; }

    int htop(int v) const { return hld[v].htop; }
    int hbot(int v) const { return hld[htop(v)].hbot; }
    pii hpath(int v) const {
        return make_pair(index(htop(v)), index(hbot(v)) + 1);
    }

    int edge_index(int eid) const {
        const E& e = this->t.edges[eid];
        return index(depth(e.u) > depth(e.v) ? e.u : e.v);
    }

    int kth_ancestor(int u, int k) const {
        assert(0 <= k && k <= depth(u));
        while (true) {
            if (k <= depth(u) - depth(htop(u)))
                return at_index[index(u) - k];
            k -= depth(u) - depth(htop(u)) + 1;
            u = par(htop(u));
        }
    }

    // Returns the kth node on the simple path from u to v
    int kth_step(int u, int v, int k) const {
        int w = this->lca(u, v), d = this->dist(u, v);
        assert(d >= k);
        return k <= depth(u) - depth(w) ? kth_ancestor(u, k) : kth_ancestor(v, d - k);
    }

    void decompose_vertical_path(int u, int v, bool up, vpii& res) const {
        size_t bef = res.size();
        for (assert(depth(u) >= depth(v)); true; u = par(u)) {
            int w = depth(htop(u)) >= depth(v) ? htop(u) : v;
            res.emplace_back(index(up ? u : w), index(up ? w : u));
            if ((u = w) == v) break;
        }
        if (!up) reverse(res.begin() + bef, res.end());
    }

    vpii decompose_path(int u, int v, bool include_lca) const {
        vpii res;
        int w = this->lca(u, v);
        if (u != w) decompose_vertical_path(u, this->first_step(w, u), true, res);
        if (include_lca) res.emplace_back(index(w), index(w));
        if (v != w) decompose_vertical_path(v, this->first_step(w, v), false, res);
        return res;
    }

    template<typename FOR, typename FORd>
    void for_each(int u, int v, bool include_lca, FOR f, FORd fd) const {
        for (pii r : decompose_path(u, v, include_lca)) {
            if (r.f > r.s) fd(r.s, r.f + 1);
            else f(r.f, r.s + 1);
        }
    }
    template<typename FOR>
    void for_each_down(int u, int v, bool include_lca, FOR f) const {
        assert(this->lca(u, v) == u);
        auto fd = [&f](int i, int j){ assert(j - i <= 1); return f(i, j); };
        for_each(u, v, include_lca, f, /*unused*/fd);
    }
    template<typename FORd>
    void for_each_up(int u, int v, bool include_lca, FORd fd) const {
        assert(this->lca(u, v) == v);
        auto f = [&fd](int i, int j){ assert(j - i <= 1); return fd(i, j); };
        for_each(u, v, include_lca, /*unused*/f, fd);
    }
    template<typename FOR>
    void for_each_commutative(int u, int v, bool include_lca, FOR f) const {
        for_each(u, v, include_lca, f, f);
    }

    template<typename T, typename OP, typename FOLDL, typename FOLDR>
    T accumulate(int u, int v, bool include_lca, T iv, OP lplus, FOLDL fl, FOLDR fr) const {
        for (pii r : decompose_path(u, v, include_lca)) {
            if (r.f > r.s) iv = lplus(iv, fr(r.s, r.f + 1));
            else iv = lplus(iv, fl(r.f, r.s + 1));
        }
        return iv;
    }
    template<typename T, typename OP, typename FOLDL>
    T accumulate_down(int u, int v, bool include_lca, T iv, OP lplus, FOLDL fl) const {
        assert(this->lca(u, v) == u);
        auto fr = [&fl](int i, int j){ assert(j - i <= 1); return fl(i, j); };
        return accumulate(u, v, include_lca, iv, lplus, fl, /*unused*/fr);
    }
    template<typename T, typename OP, typename FOLDR>
    T accumulate_up(int u, int v, bool include_lca, T iv, OP lplus, FOLDR fr) const {
        assert(this->lca(u, v) == v);
        auto fl = [&fr](int i, int j){ assert(j - i <= 1); return fr(i, j); };
        return accumulate(u, v, include_lca, iv, lplus, /*unused*/fl, fr);
    }
    template<typename T, typename OP, typename FOLD>
    T accumulate_commutative(int u, int v, bool include_lca, T iv, OP lplus, FOLD f) const {
        return accumulate(u, v, include_lca, iv, lplus, f, f);
    }
};

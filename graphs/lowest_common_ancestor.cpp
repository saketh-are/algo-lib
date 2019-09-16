template<typename E> struct lowest_common_ancestor {
    tree<E> t;
    int root;
    vi _par, _depth, _subt_sz;

    const vi& nbrs(int v) const { return t.nbrs[v]; }
    int par(int v) const { return _par[v]; }
    int depth(int v) const { return _depth[v]; }
    int subt_sz(int v) const { return _subt_sz[v]; }

    struct visit { int node, depth, index; };
    struct adder {
        visit operator()(const visit& a, const visit& b) const {
            if (a.depth != b.depth) return a.depth < b.depth ? a : b;
            return a.index > b.index ? a : b;
        }
    } vv;

    vector<visit> euler_tour;
    vi first_visit;
    sparse_table<visit, adder> table;

    lowest_common_ancestor() {}
    lowest_common_ancestor(tree<E> _t, int _root = 0) : t(_t), root(_root) {
        _par.resz(t.V), _depth.resz(t.V), _subt_sz.resz(t.V), first_visit.resz(t.V);
        auto dfs = [&](auto& self, int loc) -> void {
            _subt_sz[loc] = 1;
            first_visit[loc] = sz(euler_tour);
            euler_tour.pb({loc, _depth[loc], sz(euler_tour)});

            for (int nbr : this->nbrs(loc)) if (nbr != this->par(loc)) {
                _par[nbr] = loc;
                _depth[nbr] = this->depth(loc) + 1;
                self(self, nbr);
                _subt_sz[loc] += this->subt_sz(nbr);
                euler_tour.push_back({loc, this->depth(loc), sz(euler_tour)});
            }
        };
        dfs(dfs, root);
        table = sparse_table<visit, adder>(euler_tour, vv);
    }

    int lca(int u, int v) const {
        u = first_visit[u], v = first_visit[v];
        if (u > v) swap(u, v);
        return table(u, v + 1).node;
    }

    int dist(int u, int v) const {
        return depth(u) + depth(v) - 2 * depth(lca(u, v));
    }

    bool uv_path_has_w(int u, int v, int w) const {
        return dist(u, v) == (dist(u, w) + dist(w, v));
    }

    // Returns the neighbor of u on the simple path from u to v
    int first_step(int u, int v) const {
        assert(u != v);
        if (lca(u, v) != u) return par(u);
        return euler_tour[table(first_visit[u], first_visit[v]).index + 1].node;
    }

    template<typename T, typename F> vector<T> prefix_sums(T id, F op) {
        vector<T> res(t.V, id);
        auto dfs = [&](auto& self, int loc) -> void {
            for (int eid : t.eids[loc]) {
                E& e = t.edges[eid]; int nbr = e[loc];
                if (nbr == par(loc)) continue;
                res[nbr] = op(res[loc], e);
                self(self, nbr);
            }
        };
        dfs(dfs, root);
        return res;
    }
    template<typename T> T read_path(vector<T> prefix_sums, int u, int v) {
        return prefix_sums[u] + prefix_sums[v] - 2 * prefix_sums[lca(u, v)];
    }
};

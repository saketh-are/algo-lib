/*
 * Supports O(1) lowest common ancestor queries on an immutable forest.
 */
struct lowest_common_ancestor {
    struct visit {
        int node, depth, index;
        static visit higher(const visit& a, const visit& b) {
            if (a.depth != b.depth) return a.depth < b.depth ? a : b;
            return a.index > b.index ? a : b;
        }
    };

    int V;
    vector<visit> tour;
    sparse_table<visit> table;
    vi first, depth, par, comp;

    lowest_common_ancestor(const vvi& tree = {}, vi roots = {}) : V(sz(tree)) {
        depth.resz(V), comp.resz(V, -1), first.resz(V, -1), par.resz(V, -1);

        int cc = 0;
        auto dfs = [&](auto& self, int loc) -> void {
            comp[loc] = cc, first[loc] = sz(tour);
            tour.push_back({loc, depth[loc], sz(tour)});
            for (int nbr : tree[loc]) if (nbr != par[loc]) {
                par[nbr] = loc;
                depth[nbr] = depth[loc] + 1;
                self(self, nbr);
                tour.push_back({loc, depth[loc], sz(tour)});
            }
        };
        for (int i : roots) {
            assert(comp[i] == -1);
            dfs(dfs, i), cc++;
        }
        for (int i = 0; i < V; i++) {
            if (comp[i] == -1) dfs(dfs, i), cc++;
        }

        table = sparse_table<visit>(tour, visit::higher);
    }

    // Returns the lowest common ancestor of u and v
    // Returns -1 if they are in different components
    int lca(int u, int v) {
        if (comp[u] != comp[v]) return -1;
        u = first[u], v = first[v];
        if (u > v) swap(u, v);
        return table(u, v + 1).node;
    }

    int dist(int u, int v) {
        assert(comp[u] == comp[v]);
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    // Checks if btw is on the path from u to v
    bool on_path(int u, int v, int btw) {
        return dist(u, v) == (dist(u, btw) + dist(btw, v));
    }

    // Returns the neighbor of u on the shortest path to v
    int first_step(int u, int v) {
        assert(u != v && comp[u] == comp[v]);
        int w = lca(u, v);
        return u == w ? tour[table(first[u], first[v]).index+1].node : par[u];
    }
};

// Extension for weighted trees to support path weight queries.
struct weighted_lowest_common_ancestor {
    int V;
    vll weighted_depth;
    vi root;
    lowest_common_ancestor lca;
    weighted_lowest_common_ancestor(const vvpii& tree = {}, vi roots = {}) : V(sz(tree)) {
        vvi adj(V);
        for (int v = 0; v < sz(tree); v++)
            for (pii e : tree[v])
                adj[v].push_back(e.first);
        lca = lowest_common_ancestor(adj, roots);

        weighted_depth.resize(V, LLONG_MIN);
        root.resize(V);

        int cc = 0;
        auto dfs = [&](auto& self, int loc, int par, int rt) -> void {
            root[loc] = rt;
            for (pii e : tree[loc]) if (e.first != par) {
                weighted_depth[e.first] = weighted_depth[loc] + e.second;
                self(self, e.first, loc, rt);
            }
        };
        for (int i : roots) {
            assert(weighted_depth[i] == LLONG_MIN);
            weighted_depth[i] = 0;
            dfs(dfs, i, i, i), cc++;
        }
        for (int i = 0; i < V; i++) {
            if (weighted_depth[i] == LLONG_MIN) {
                weighted_depth[i] = 0;
                dfs(dfs, i, i, i), cc++;
            }
        }
    }

    ll path_weight(int u, int v) {
        assert(lca.comp[u] == lca.comp[v]);
        return weighted_depth[u] + weighted_depth[v] - 2 * weighted_depth[lca.lca(u, v)];
    }
};

/*
 * Supports O(1) lowest common ancestor queries on an immutable forest.
 */
struct lowest_common_ancestor {
    struct visit {
        int node, depth, index;
        visit operator +(const visit& o) const {
            if (depth != o.depth) return depth < o.depth ? *this : o;
            return index > o.index ? *this : o;
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
            if (comp[i] == -1)
                dfs(dfs, i), cc++;
        }

        table = sparse_table<visit>(tour);
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

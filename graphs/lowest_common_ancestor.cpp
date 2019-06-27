/*
 * Supports O(1) lowest common ancestor queries on an immutable forest.
 */
struct lowest_common_ancestor {
    struct visit {
        int node, depth;
        visit operator + (const visit& o) const {
            return depth <= o.depth ? *this : o;
        }
    };

    vi depth, pos, comp;
    sparse_table<visit> euler;

    lowest_common_ancestor(const vvi& tree = {}, int root = -1) {
        depth.resize(sz(tree));
        comp.resize(sz(tree), -1);
        pos.resize(sz(tree), -1);

        vector<visit> tour; int cc = 0;
        auto dfs = [&](auto& self, int loc, int par) -> void {
            comp[loc] = cc, pos[loc] = sz(tour);
            tour.push_back({ loc, depth[loc] });
            for (int nbr : tree[loc]) if (nbr != par) {
                depth[nbr] = depth[loc] + 1;
                self(self, nbr, loc);
                tour.push_back({ loc, depth[loc] });
            }
        };
        if (root != -1) dfs(dfs, root, root), cc++;
        for (int i = 0; i < sz(tree); i++)
            if (comp[i] == -1) dfs(dfs, i, i), cc++;
        euler = sparse_table<visit>(tour);
    }

    // Returns the lowest common ancestor of u and v
    // Returns -1 if they are in different components
    int lca(int u, int v) {
        if (comp[u] != comp[v]) return -1;
        u = pos[u], v = pos[v];
        if (u > v) swap(u, v);
        return euler.query(u, v).node;
    }

    int dist(int u, int v) {
        assert(comp[u] == comp[v]);
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    // Checks if btw is on the path from u to v
    bool on_path(int u, int v, int btw) {
        return dist(u, v) == (dist(u, btw) + dist(btw, v));
    }
};

/*
 * Supports O(1) lowest common ancestor queries on an immutable tree.
 */
struct lowest_common_ancestor {
    struct visit {
        int node, depth;
        visit operator + (const visit& o) const {
            return depth <= o.depth ? *this : o;
        }
    };

    vi depth, pos;
    sparse_table<visit> euler;

    // Constructs the lookup table in O(V log V) time and memory
    lowest_common_ancestor(const vvi& tree = {}, int root = 0) {
        depth.resize(tree.size());
        pos.resize(tree.size());
        vector<visit> tour;
        if (tree.size()) dfs(tree, tour, root, root);
        euler = sparse_table<visit>(tour);
    }

    void dfs(const vvi& tree, auto& tour, int loc, int par) {
        pos[loc] = tour.size();
        tour.push_back({ loc, depth[loc] });
        for (int nbr : tree[loc]) if (nbr != par) {
            depth[nbr] = depth[loc] + 1;
            dfs(tree, tour, nbr, loc);
            tour.push_back({ loc, depth[loc] });
        }
    }

    // Returns the lowest common ancestor of u and v in O(1)
    int lca(int u, int v) {
        u = pos[u], v = pos[v];
        if (u > v) swap(u, v);
        return euler.query(u, v).node;
    }

    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    bool on_path(int u, int v, int w) {
        return dist(u, v) == (dist(u, w) + dist(w, v));
    }
};


/*
 * Supports O(logN) lowest common ancestor queries on an immutable tree.
 */
struct lowest_common_ancestor_log {
    int L, N;
    vi depth, link;

    lowest_common_ancestor_log() { }

    lowest_common_ancestor_log(const vvi &graph, int root = 0) : N(graph.size()) {
        L = 32 - __builtin_clz(N);
        depth.resize(N);
        link.resize(L*N);
        init(root, root, graph);
    }

    /*
     * Initializes the link table in O(NlogN). link[l * N + i] contains the index
     * of the (2 ** l)th ancestor of vertex i.
     */
    void init(int loc, int par, const vvi &graph) {
        link[loc] = par;
        for (int l = 1; l < L; l++)
            link[l*N + loc] = link[(l-1)*N + link[(l-1)*N + loc]];

        for (int nbr : graph[loc]) if (nbr != par) {
            depth[nbr] = depth[loc] + 1;
            init(nbr, loc, graph);
        }
    }

    // Returns the index of the dist-th ancestor of vertex loc in O(logN).
    int above(int loc, int dist) {
        dist = min(dist, N - 1);
        for (int l = 0; l < L; l++)
            if (dist & (1 << l))
                loc = link[l*N + loc];
        return loc;
    }

    // Returns the least common ancestor of vertices u and v in O(logN).
    int lca(int u, int v) {
        if (depth[u] > depth[v]) swap(u, v);
        v = above(v, depth[v] - depth[u]);
        if (u == v) return u;

        for (int l = L - 1; l >= 0; l--) {
            if (link[l*N + u] != link[l*N + v])
                u = link[l*N + u], v = link[l*N + v];
        }
        return link[u];
    }

    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    }

    bool on_path(int u, int v, int inx) {
        return dist(u, v) == dist(u, inx) + dist(inx, v);
    }
};

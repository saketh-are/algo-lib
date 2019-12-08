template<typename E, int MAXV, bool PREORDER>
struct centroid_decomposition {
    static const int L = 33 - __builtin_clz(MAXV - 1);

    const tree<E>& t;
    using P = typename E::path;
    struct visit { int node; E in; };
    vi layer;                        // 0-indexed depth in the centroid tree
    vpii par;                        // {parent, index among parent's children}
    vi child_ct;                     // number of children
    vector<array<P, L>> anc_path;    // paths to ancestors, by layer
    vector<vector<visit>> preorder;  // preorder traversals recording {node, incoming edge}
    vvi ch_sz;                       // child tree sizes in traversal order

    centroid_decomposition(const tree<E>& _t) : t(_t),
            layer(t.V, -1), par(t.V), child_ct(t.V), anc_path(t.V) {
        assert(t.V <= MAXV);
        if (PREORDER) preorder.resz(t.V), ch_sz.resz(t.V);
        subt_sz = t.subt_sz, decompose(t.root), subt_sz.clear();
    }

    vi subt_sz;
    int centroid(int r) {
        int c = r;
        FIND: for (E e : t.nbrs[c]) if (int u = e(c); layer[u] == -1) {
            if (subt_sz[u] < subt_sz[c] && 2 * subt_sz[u] >= subt_sz[r]) {
                c = u; goto FIND;
            }
        }
        return c;
    }

    int ly = 0, rt = 0;
    void traverse(int u, E in) {
        if (PREORDER) preorder[rt].pb({u, in});
        subt_sz[u] = 1;
        for (E e : t.nbrs[u]) if (int v = e(u); v != in(u) && layer[v] == -1) {
            anc_path[v][ly] = anc_path[u][ly] + P(e);
            traverse(v, e);
            subt_sz[u] += subt_sz[v];
        }
    }

    void decompose(int r = 0, int p = -1, int i = 0) {
        par[r = centroid(r)] = {p, i}, i = 0;
        layer[r] = ly;
        if (PREORDER) preorder[r] = {{r, E()}};
        subt_sz[r] = 1;
        for (E e : t.nbrs[r]) if (int u = e(r); layer[u] == -1) {
            if (PREORDER) ch_sz[r].pb(-sz(preorder[r]));
            rt = r, anc_path[u][ly] = P(e), traverse(u, e);
            if (PREORDER) ch_sz[r].back() += sz(preorder[r]);
            subt_sz[r] += subt_sz[u], child_ct[r]++;
            ly++, decompose(u, r, i++), ly--;
        }
    }

    int lca(int u, int v) const {
        if (layer[u] < layer[v]) swap(u, v);
        while (layer[u] != layer[v]) u = par[u].f;
        while (u != v) u = par[u].f, v = par[v].f;
        return u;
    }

    P dist(int u, int v) const {
        int clca = lca(u, v);
        return anc_path[u][layer[clca]] + anc_path[v][layer[clca]];
    }
};

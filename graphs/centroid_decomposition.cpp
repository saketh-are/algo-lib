template<typename E> struct centroid_decomposition {
    const tree<E>& t;

    vi layer;        // 1-indexed depth in the centroid tree
    vvi ch;          // children in centroid tree
    vpii par;        // {parent in centroid tree, index among its children}
    vi layer_order;  // preorder traversal of the centroid tree
    vvpii preorder;  // preorder traversals of centroid subtrees recording {node, traversal parent}
    vvi ch_sz;       // sizes of child trees in traversal order
    vvi anc_dist;    // distances to ancestors in the centroid tree, by layer

    centroid_decomposition(const tree<E>& _t) : t(_t), layer(t.V, -1), ch(t.V), par(t.V),
            preorder(t.V), ch_sz(t.V), anc_dist(t.V), __subt_sz(t.V) {
        decompose();
        for (int u : layer_order)
            for (pii v : preorder[u])
                anc_dist[v.f].pb(1 + (~v.s ? anc_dist[v.s].back() : -1));
    }

    vi __subt_sz;
    void calc_subt_sz(int u, int p) {
        __subt_sz[u] = 1;
        for (int v : t.nbrs[u]) if (v != p && layer[v] == -1) {
            calc_subt_sz(v, u);
            __subt_sz[u] += __subt_sz[v];
        }
    }
    pii centroids(int r) {
        calc_subt_sz(r, -1);
        int c = r, p = -1;
        FIND: for (int u : t.nbrs[c]) if (layer[u] == -1) {
            if (__subt_sz[u] < __subt_sz[c] && 2 * __subt_sz[u] >= __subt_sz[r]) {
                p = c, c = u; goto FIND;
            }
        }
        return __subt_sz[c] * 2 == __subt_sz[r] ? pii{c,p} : pii{c,-1};
    }

    void calc_preorder(int u, int p, int r) {
        preorder[r].pb({u, p});
        if (u == r) ch_sz[r] = {1};
        for (int v : t.nbrs[u]) if (v != p && layer[v] == -1) {
            if (u == r) ch_sz[r].pb(-sz(preorder[r]));
            calc_preorder(v, u, r);
            if (u == r) ch_sz[r].back() += sz(preorder[r]);
        }
    }
    int decompose(int r = 0, int p = -1, int pi = 0, int ly = 1) {
        par[r = centroids(r).f] = {p, pi};
        layer[r] = ly, layer_order.pb(r);
        calc_preorder(r, -1, r);
        for (int u : t.nbrs[r]) if (layer[u] == -1) {
            ch[r].pb(decompose(u, r, sz(ch[r]), ly + 1));
        }
        return r;
    }
};

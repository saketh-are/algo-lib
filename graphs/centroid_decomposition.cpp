template<typename E> struct centroid_decomposition {
    const tree<E>& t;

    vi layer;
    vvi ch;   // children in centroid tree
    vpii par; // parent in centroid tree, index among its children
    vvi dist; // distances to ancestors in centroid tree, by layer

    centroid_decomposition(const tree<E>& _t) : t(_t),
            layer(t.V, -1), ch(t.V), par(t.V), dist(t.V) {
        subt_sz.resz(t.V);
        decompose();
    }

    vi subt_sz;
    void calc_subt_sz(int u, int p) {
        subt_sz[u] = 1;
        for (int v : t.nbrs[u]) if (v != p && layer[v] == -1) {
            calc_subt_sz(v, u);
            subt_sz[u] += subt_sz[v];
        }
    }
    pii centroids(int r) {
        calc_subt_sz(r, -1);
        int c = r, p = -1;
        FIND: for (int u : t.nbrs[c]) if (layer[u] == -1) {
            if (subt_sz[u] < subt_sz[c] && 2 * subt_sz[u] >= subt_sz[r]) {
                p = c, c = u; goto FIND;
            }
        }
        return subt_sz[c] * 2 == subt_sz[r] ? pii{c,p} : pii{c,-1};
    }

    void calc_dist(int u, int p, int d) {
        dist[u].pb(d);
        for (int v : t.nbrs[u]) if (v != p && layer[v] == -1) {
            calc_dist(v, u, d + 1);
        }
    }
    int decompose(int r = 0, int p = -1, int pi = 0, int ly = 0) {
        par[r = centroids(r).f] = {p, pi}, layer[r] = ly;
        calc_dist(r, -1, 0);
        for (int u : t.nbrs[r]) if (layer[u] == -1) {
            ch[r].pb(decompose(u, r, sz(ch[r]), ly + 1));
        }
        return r;
    }
};

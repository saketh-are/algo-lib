template<typename E> struct tree {
    int V;
    vvi nbrs;
    vector<E> edges;
    vector<vector<E>> adj;

    vi subt_sz;
    vb erased;

    tree() {}
    tree(int _V) : V(_V) {
        edges.resz(V-1);
        nbrs.resz(V), adj.resz(V), subt_sz.resz(V), erased.resz(V);
    }
    tree(const vector<E>& __edges) : tree(sz(__edges) + 1) {
        edges = __edges;
        init();
    }

    void init() {
        F0R (i, sz(edges)) {
            auto& e = edges[i]; e.i = i;
            assert(0 <= e.u && e.u < V && 0 <= e.v && e.v < V);
            adj[e.u].pb(e), nbrs[e.u].pb(e.v);
            adj[e.v].pb(e), nbrs[e.v].pb(e.u);
        }
    }

    friend void re(tree& t) { re(t.edges); t.init(); }
    friend void pr(const tree& t) { pr("{V=", t.V, " ", t.edges, "}"); }

    void erase(int v) { erased[v] = true; }

    void calc_subt_sz(int v, int p = -1) {
        subt_sz[v] = 1;
        trav (e, adj[v]) {
            int u = e[v];
            if (erased[u] || u == p) continue;
            calc_subt_sz(u, v);
            subt_sz[v] += subt_sz[u];
        }
    }

    int centroid(int v) {
        calc_subt_sz(v);
        int c = v;
        FIND: trav (e, adj[c]) {
            int u = e[c];
            if (!erased[u] && subt_sz[u] < subt_sz[c] && 2 * subt_sz[u] >= subt_sz[v]) {
                c = u; goto FIND;
            }
        }
        return c;
    }
};

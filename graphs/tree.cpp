template<typename E> struct tree {
    int V;
    vvi nbrs;
    vector<vector<reference_wrapper<E>>> edges;
    vector<E> edge_list;

    tree(int _V = 0) : V(_V) {}
    tree(const vector<E>& __edge_list) : V(sz(__edge_list) + 1), edge_list(__edge_list) {
        nbrs.resize(V), edges.resize(V);
        for (E& e : edge_list) {
            assert(0 <= e.u && e.u < V && 0 <= e.v && e.v < V);
            nbrs[e.u].push_back(e.v);
            nbrs[e.v].push_back(e.u);
            edges[e.u].pb(e);
            edges[e.v].pb(e);
        }
        erased.resize(V), subt_sz.resize(V);
    }

    friend void re(tree& t) {
        assert(t.V > 0);
        vector<E> __edge_list(t.V - 1);
        re(__edge_list);
        t = tree<E>(__edge_list);
    }
    friend void pr(const tree& t) { pr("{V=", t.V, " ", t.edge_list, "}"); }

    vb erased;
    void erase(int u) { erased[u] = true; }

    mutable vi subt_sz;
    void calc_subt_sz(int u, bool avoid_erased, int p) const {
        subt_sz[u] = 1;
        for (int v : nbrs[u]) if (v != p && (!avoid_erased || !erased[v])) {
            calc_subt_sz(v, avoid_erased, u);
            subt_sz[u] += subt_sz[v];
        }
    }

    vi centroids(int r = 0, bool avoid_erased = 1) const {
        calc_subt_sz(r, avoid_erased, -1);
        int c = r, p = -1;
        FIND: for (int u : nbrs[c]) if (!avoid_erased || !erased[u]) {
            if (subt_sz[u] < subt_sz[c] && 2 * subt_sz[u] >= subt_sz[r]) {
                p = c, c = u; goto FIND;
            }
        }
        return subt_sz[c] * 2 == subt_sz[r] ? vi{c,p} : vi{c};
    }
};

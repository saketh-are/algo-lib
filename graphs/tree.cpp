template<typename E> struct tree {
    int V;
    vvi nbrs, eids;
    vector<E> edges;

    tree() {}
    tree(int _V) : V(_V) {
        nbrs.resz(V), eids.resz(V), edges.resz(V-1);
    }
    tree(const vector<E>& __edges) : V(sz(__edges) + 1), edges(__edges)  {
        nbrs.resz(V), eids.resz(V);
        init();
    }

    void init() {
        F0R (i, sz(edges)) {
            auto& e = edges[i];
            assert(0 <= e.u && e.u < V && 0 <= e.v && e.v < V);

            nbrs[e.u].pb(e.v);
            nbrs[e.v].pb(e.u);

            e.i = i;
            eids[e.u].pb(i);
            eids[e.v].pb(i);
        }
    }

    friend void re(tree& t) { re(t.edges); t.init(); }
    friend void pr(const tree& t) { pr("{V=", t.V, " ", t.edges, "}"); }
};

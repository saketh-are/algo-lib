template<typename E> struct graph {
    int V, EC;
    vvi nbrs;
    vector<vector<reference_wrapper<E>>> edges;
    vector<E> edge_list;

    graph(int _V = 0, int _EC = 0) : V(_V), EC(_EC) {}
    graph(int _V, const vector<E>& __edge_list) : V(_V), EC(sz(__edge_list)), edge_list(__edge_list) {
        nbrs.resize(V), edges.resize(V);
        for (E& e : edge_list) {
            assert(0 <= e.u && e.u < V && 0 <= e.v && e.v < V);
            nbrs[e.u].push_back(e.v);
            nbrs[e.v].push_back(e.u);
            edges[e.u].pb(e);
            edges[e.v].pb(e);
        }
    }

    friend void re(graph& g) {
        assert(g.V > 0);
        vector<E> __edge_list(g.EC);
        re(__edge_list);
        for (int i = 0; i < g.EC; i++) __edge_list[i].i = i;
        g = graph<E>(__edge_list);
    }
    friend void pr(const graph& g) { pr("{V=", g.V, " ", g.edge_list, "}"); }
};

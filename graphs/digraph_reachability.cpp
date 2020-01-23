template<int MAXV>
vector<bitset<MAXV>> digraph_reachability(const vvi& graph) {
    int V = sz(graph);
    assert(V <= MAXV);

    int C; vi comp; tie(C, comp) = strongly_connected_components(graph);

    vector<bitset<MAXV>> comp_reach(C);
    for (int v = 0; v < V; v++) comp_reach[comp[v]][v] = 1;

    vi order(V);
    for (int v = 0; v < V; v++) order[v] = v;
    sort(all(order), [&](int u, int v) { return comp[u] < comp[v]; });
    for (int u : order) for (int v : graph[u]) {
        comp_reach[comp[u]] |= comp_reach[comp[v]];
    }

    vector<bitset<MAXV>> reach(V);
    for (int v = 0; v < V; v++) reach[v] = comp_reach[comp[v]];
    return reach;
}

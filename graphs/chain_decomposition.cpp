template<typename E> struct chain_decomposition {
    const graph<E>& g;

    vvi chains;
    vb artp, bridge;

    chain_decomposition(const graph<E>& _g) : g(_g) {
        vi dfs_order, dfs_index(g.V, -1), parent_edge(g.V, -1);
        vector<vector<reference_wrapper<const E>>> back_edges(g.V);

        vb seen_edge(g.EC, false);
        auto dfs = [&](auto& self, int u) -> void {
            dfs_index[u] = sz(dfs_order);
            dfs_order.pb(u);
            for (const E& e : g.edges[u]) if (!seen_edge[e.i]) {
                seen_edge[e.i] = true;
                if (dfs_index[e[u]] == -1) {
                    parent_edge[e[u]] = e.i;
                    self(self, e[u]);
                } else back_edges[e[u]].pb(e);
            }
        };
        for (int u = 0; u < g.V; u++) if (dfs_index[u] == -1) dfs(dfs, u);

        artp.resize(g.V, false);
        bridge.resize(g.EC, true);

        vb visited(g.V, false);
        for (int u : dfs_order) for (const E& e : back_edges[u]) {
            chains.pb({e.i});
            int v = e[u];
            for (visited[u] = true; !visited[v]; ) {
                visited[v] = true;
                chains.back().pb(parent_edge[v]);
                v = g.edge_list[parent_edge[v]][v];
            }
            for (int edge_index : chains.back()) bridge[edge_index] = false;
            if (sz(chains) > 1 && u == v) artp[u] = true;
        }

        for (const E& e : g.edge_list) if (bridge[e.i]) {
            if (sz(g.nbrs[e.u]) >= 2) artp[e.u] = true;
            if (sz(g.nbrs[e.v]) >= 2) artp[e.v] = true;
        }
    }

    bool is_articulation_point(int vertex) const {
        return artp[vertex];
    }
    bool is_bridge(int edge_index) const {
        return bridge[edge_index];
    }
};

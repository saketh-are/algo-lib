template<typename F> struct dinic {
    static const F inf = numeric_limits<F>::max();

    int V;
    vvi adj;
    vi dest;
    vector<F> cap;

    dinic (int V = 0) : V(V) {
        adj.resize(V);
    }

    void __arc(int u, int v, F c) {
        adj[u].push_back(dest.size());
        dest.push_back(v);
        cap.push_back(c);
    }

    // Inserts a directed edge u --> v with capacity c.
    void arc(int u, int v, F c) {
        __arc(u, v, c);
        __arc(v, u, F(0));
    }

    bool bfs(int s, int t, vi& level, vector<F>& flow) const {
        level = vi(V, -1);
        level[s] = 0;
        for (queue<int> q({s}); !q.empty(); q.pop()) {
            int u = q.front();
            for (int e : adj[u]) {
                if (level[dest[e]] == -1 && flow[e] < cap[e]) {
                    level[dest[e]] = level[u] + 1;
                    q.push(dest[e]);
                }
            }
        }
        return level[t] != -1;
    }

    F augment(int s, int t, vi& level, vector<F>& flow, vi& inx, F cur) const {
        if (s == t) return cur;
        for (int e; inx[s] < adj[s].size(); inx[s]++) {
            e = adj[s][inx[s]];
            if (level[dest[e]] != level[s] + 1) continue;
            if (flow[e] == cap[e]) continue;
            F incr = augment(dest[e], t, level, flow, inx, min(cur, cap[e] - flow[e]));
            if (incr > F(0)) {
                flow[e] += incr;
                flow[e^1] -= incr;
                return incr;
            }
        }
        return F(0);
    }

    /*
     * Computes a maximum flow from node s to node t.
     *
     * Runs in O(V^2 * E) in the general case.
     * Runs in O(min{ V^(2/3), E^(1/2) } * E) if all edges have unit capacity.
     * Runs in O(V^(1/2) * E) for bipartite matching.
     */
    tuple<F, vector<F>> max_flow(int s, int t) const {
        assert(s != t);
        F res(0);
        vector<F> flow(cap.size());
        for (vi level; bfs(s, t, level, flow); ) {
            for (vi inx(V, 0); F incr = augment(s, t, level, flow, inx, inf); )
                res += incr;
        }
        return make_tuple(res, flow);
    }
};


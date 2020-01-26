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
    int arc(int u, int v, F c) {
        __arc(u, v, c);
        __arc(v, u, F(0));
        return sz(dest) - 2;
    }

    mutable vi level;
    void bfs(int s, vector<F>& flow) const {
        level.resz(V), fill(all(level), -1), level[s] = 0;
        for (queue<int> q({s}); !q.empty(); q.pop()) {
            for (int e : adj[q.front()]) {
                if (level[dest[e]] == -1 && flow[e] < cap[e]) {
                    level[dest[e]] = level[q.front()] + 1;
                    q.push(dest[e]);
                }
            }
        }
    }

    mutable vi inx;
    F augment(int s, int t, vector<F>& flow, F cur = inf) const {
        if (s == t) return cur;
        for (; inx[s] < adj[s].size(); inx[s]++) {
            int e = adj[s][inx[s]];
            if (level[dest[e]] != level[s] + 1) continue;
            if (flow[e] == cap[e]) continue;
            if (F incr = augment(dest[e], t, flow, min(cur, cap[e] - flow[e]))) {
                flow[e] += incr;
                flow[e^1] -= incr;
                return incr;
            }
        }
        return 0;
    }

    /*
     * Computes a maximum flow from node s to node t.
     *
     * Runs in O(V^2 * E) in the general case.
     * Runs in O(min{ V^(2/3), E^(1/2) } * E) if all edges have unit capacity.
     * Runs in O(V^(1/2) * E) for bipartite matching.
     */
    struct max_flow {
        F res;
        vector<F> flow;
    };
    max_flow solve(int s, int t) const {
        assert(s != t);
        F res = 0;
        vector<F> flow(cap.size());
        while (bfs(s, flow), ~level[t]) {
            inx.resz(V), fill(all(inx), 0);
            while (F incr = augment(s, t, flow))
                res += incr;
        }
        return max_flow{res, flow};
    }
};

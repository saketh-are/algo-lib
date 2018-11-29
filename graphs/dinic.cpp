template<typename F> struct dinic {
    F ZERO, INF;

    int V;
    vvi adj;
    vi dest;
    vector<F> cap;

    dinic (int _V = 0, F _zero = 0, F _inf = INT_MAX) :
            V(_V), ZERO(_zero), INF(_inf) {
        adj.resize(V);
    }

    void __arc(int u, int v, F _cap) {
        adj[u].push_back(dest.size());
        dest.push_back(v);
        cap.push_back(_cap);
    }

    void arc(int u, int v, F _cap) {
        __arc(u, v, _cap);
        __arc(v, u, ZERO);
    }

    bool bfs(int s, int t, vi& level, vector<F>& flow) {
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

    F augment(int s, int t, vi& level, vector<F>& flow, vi& inx, F cur) {
        if (s == t) return cur;
        for (int e; inx[s] < adj[s].size(); inx[s]++) {
            e = adj[s][inx[s]];
            if (level[dest[e]] != level[s] + 1) continue;
            if (flow[e] == cap[e]) continue;
            F incr = augment(dest[e], t, level, flow, inx, min(cur, cap[e] - flow[e]));
            if (incr > ZERO) {
                flow[e] += incr;
                flow[e^1] -= incr;
                return incr;
            }
        }
        return ZERO;
    }

    // O(V^2 E)
    F max_flow(int s, int t) {
        assert(s != t);
        F res = ZERO;
        vector<F> flow(cap.size());
        for (vi level; bfs(s, t, level, flow); ) {
            for (vi inx(V, 0); F incr = augment(s, t, level, flow, inx, INF); )
                res += incr;
        }
        return res;
    }
};


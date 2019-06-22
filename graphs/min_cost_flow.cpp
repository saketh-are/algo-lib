template<typename F, typename C> struct min_cost_flow {
    static constexpr int bits = 63 - __builtin_clzll(numeric_limits<F>::max());

    int V, E;
    vvi adj;
    vi dest;
    vector<F> cap;
    vector<C> cost;

    min_cost_flow(int V = 0) : V(V), E(0) {
        adj.resize(V);
    }

    void __arc(int u, int v, F f, C c) {
        E++;
        adj[u].push_back(dest.size());
        dest.push_back(v);
        cap.push_back(f);
        cost.push_back(c);
    }

    // Inserts a directed edge u --> v with capacity f and cost c.
    void arc(int u, int v, F f, C c) {
        __arc(u, v, f, c);
        __arc(v, u, F(0), -c);
    }

    bool dijkstra(auto& imb, auto& flow, auto& pot, F delta) const {
        priority_queue<tuple<C, int, int>> q;
        vi ent(V, -2);
        vector<C> dist(V, numeric_limits<C>::max());
        for (int v = 0; v < V; v++) if (imb[v] >= delta) {
            dist[v] = 0;
            q.push(make_tuple(0., v, -1));
        }

        while (!q.empty()) {
            C d; int v, f; tie(d, v, f) = q.top(); q.pop();
            if (ent[v] != -2) continue;
            dist[v] = -d; ent[v] = f;
            for (int e : adj[v]) if (cap[e] - flow[e] >= delta) {
                double cd = dist[v] + (cost[e] + pot[v] - pot[dest[e]]);
                if (cd < dist[dest[e]]) {
                    dist[dest[e]] = cd;
                    q.push(make_tuple(-cd, dest[e], e ));
                }
            }
        }

        for (int v = 0; v < V; v++) if (ent[v] != -2 && imb[v] <= -delta) {
            for (int u = 0; u < V; u++)
                if (ent[u] != -2) pot[u] += dist[u];
            for (int e = ent[v]; ~e; e = ent[dest[e^1]]) {
                flow[e] += delta;
                flow[e^1] -= delta;
                imb[dest[e]] += delta;
                imb[dest[e^1]] -= delta;
            }
            return true;
        }
        return false;
    }

    // Computes the minimum cost to satisfy the specified imbalances.
    // Runs in O(E^2 * log V * log inf).
    pair<bool, C> solve(vector<F> imb) const {
        vector<F> flow(E);
        vector<C> pot(V);
        for (F delta = 1ll << bits; delta; delta >>= 1) {
            for (int e = 0; e < E; e++) {
                int u = dest[e^1], v = dest[e];
                F res = cap[e] - flow[e];
                if (res >= delta && cost[e] + pot[u] - pot[v] < 0) {
                    flow[e^1] -= res;
                    flow[e] += res;
                    imb[u] -= res;
                    imb[v] += res;
                }
            }
            while (dijkstra(imb, flow, pot, delta));
        }

        C ans = 0;
        for (int e = 0; e < E; e++) if (flow[e] > 0)  {
            ans += flow[e] * cost[e];
        }
        return { imb == vector<F>(V, 0), ans };
    }
};

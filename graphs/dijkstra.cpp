struct dijkstra {
    using EDGE_WT = int;
    using PATH_WT = int;

    PATH_WT add(PATH_WT p, EDGE_WT w) {
        static_assert(false, "Implement me! (p + w)");
    }

    bool less_than(PATH_WT x, PATH_WT y) {
        static_assert(false, "Implement me! (x < y)");
    }

    int N;
    vector<vector<pair<int, EDGE_WT>>> adj;

    dijkstra (int N_) : N(N_), adj(N_) { }

    void add_directed_edge(int u, int v, EDGE_WT w) {
        adj[u].emplace_back(v, w);
    }

    vector<int> parent;
    vector<PATH_WT> shortest_path_wt;

    void compute(const vector<int> &sources, PATH_WT zero) {
        parent.assign(N, -1);
        shortest_path_wt.assign(N, zero);

        auto cmp = [&](pair<PATH_WT, int> x, pair<PATH_WT, int> y) { return less_than(y.first, x.first); };
        priority_queue<pair<PATH_WT, int>, vector<pair<PATH_WT, int>>, decltype(cmp)> pq(cmp);

        for (int src : sources) {
            parent[src] = src;
            pq.push(make_pair(shortest_path_wt[src], src));
        }

        while (!pq.empty()) {
            PATH_WT path_wt = pq.top().first;
            int loc = pq.top().second;
            pq.pop();
            if (memcmp(&path_wt, &shortest_path_wt[loc], sizeof(PATH_WT))) continue;

            for (pair<int, EDGE_WT> edge : adj[loc]) {
                int nbr = edge.first;
                PATH_WT combined = add(path_wt, edge.second);
                if (parent[nbr] == -1 || less_than(combined, shortest_path_wt[nbr])) {
                    shortest_path_wt[nbr] = combined;
                    parent[nbr] = loc;
                    pq.push(make_pair(shortest_path_wt[nbr], nbr));
                }
            }
        }
    }

    bool is_reachable(int destination) const {
        return parent[destination] != -1;
    }

    PATH_WT distance(int destination) const {
        if (!is_reachable(destination)) assert(false);
        return shortest_path_wt[destination];
    }

    vector<int> list_vertices_on_path(int destination) const {
        if (!is_reachable(destination)) assert(false);
        vector<int> path;
        while (parent[destination] != destination) {
            path.push_back(destination);
            destination = parent[destination];
        }
        path.push_back(destination);
        reverse(all(path));
        return path;
    }
};

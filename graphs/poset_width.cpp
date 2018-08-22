vi width(vector<vi> poset) {
    int N = poset.size();
    bipartite_graph g(N, N);

    for (int i = 0; i < N; i++) {
        for (int j : poset[i])
            g.edge(j, i);
    }

    g.matching();

    vb vis[2];
    vis[false].resize(2 * N, false);
    vis[true].resize(2 * N, false);

    for (int i = 0; i < N; i++) {
        if (g.match[i] != -1) continue;
        if (vis[false][i]) continue;

        queue<pair<bool, int>> bfs;
        bfs.push(make_pair(false, i));
        vis[false][i] = true;

        while (!bfs.empty()) {
            bool inm = bfs.front().first;
            int loc = bfs.front().second;
            bfs.pop();

            for (int nbr : g.adj[loc]) {
                if (vis[!inm][nbr]) continue;
                if ((g.match[loc] == nbr) ^ inm) continue;

                vis[!inm][nbr] = true;
                bfs.push(make_pair(!inm, nbr));
            }
        }
    }

    vb inz(2 * N, false);
    for (int i = 0; i < 2 * N; i++)
        inz[i] = vis[true][i] || vis[false][i];

    vb ink(N, false);

    for (int i = 0; i < N; i++)
        if (!inz[i])
            ink[i]= true;

    for (int i = N; i < 2 * N; i++)
        if (inz[i])
            ink[i - N] = true;

    vi res;
    for (int i = 0; i < N; i++) {
        if (!ink[i])
            res.push_back(i);
    }
    return res;
}


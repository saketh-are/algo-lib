struct bipartite_graph {
    int A, B;
    vector<vector<int>> adj;

    bipartite_graph(int _A, int _B) {
        A = _A, B = _B;
        adj.resize(A + B); 
    }

    void edge(int i, int j) {
        adj[i].push_back(A+j);
        adj[A+j].push_back(i);
    }

    vector<int> visit, match;

    bool augment(int loc, int run) {
        if(visit[loc] == run) return false;
        visit[loc] = run;

        for (int nbr : adj[loc]) {
            if (match[nbr] == -1 || augment(match[nbr], run)) {
                match[loc] = nbr, match[nbr] = loc;
                return true;
            }
        }

        return false;
    }

    int matching() {
        visit = vector<int>(A+B, -1);
        match = vector<int>(A+B, -1); 

        int ans = 0;
        for (int i = 0; i < A; i++) 
            ans += augment(i, i);
        return ans;
    }
};


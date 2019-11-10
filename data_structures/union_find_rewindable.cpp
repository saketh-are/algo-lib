struct union_find_rewindable {
    struct node {
        int par, rank;
        node(int id = 0) : par(id), rank(0) {}
        void operator +=(const node& o) {
            if (rank == o.rank) rank++;
        }
    };

    int N, ops;
    vector<node> dsu;
    vector<tuple<int, node, int, int>> history;

    union_find_rewindable(int _N) : N(_N), ops(0) {
        for (int i = 0; i < N; i++)
            dsu.push_back(node(i));
    }

    int opcount() const { return ops; }
    int components() const { return N - sz(history); }

    int rep(int i) const {
        while (i != dsu[i].par)
            i = dsu[i].par;
        return i;
    }

    bool unio(int a, int b) {
        ops++;
        a = rep(a), b = rep(b);
        if(a == b) return false;

        if (dsu[a].rank < dsu[b].rank)
            swap(a, b);

        history.emplace_back(ops, dsu[a], a, b);
        dsu[a] += dsu[b];
        dsu[b].par = a;

        return true;
    }

    void rewind(int _ops) {
        for (ops = _ops; !history.empty(); history.pop_back()) {
            const auto &[op, av, a, b] = history.back();
            if (op <= ops) break;
            dsu[a] = av;
            dsu[b].par = b;
        }
    }
};

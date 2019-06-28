struct union_find {
    struct node {
        int par, rank;
        node(int id = 0) : par(id), rank(0) {}
        void operator +=(const node& o) {
            if (rank == o.rank) rank++;
        }
    };

    vector<node> uf;
    union_find(int N = 0) : uf(N) {
        for (int i = 0; i < N; i++)
            uf[i] = node(i);
    }

    int rep(int i) {
        if (i != uf[i].par)
            uf[i].par = rep(uf[i].par);
        return uf[i].par;
    }

    bool unio(int a, int b) {
        a = rep(a), b = rep(b);
        if (a == b) return false;
        if (uf[a].rank < uf[b].rank) swap(a, b);
        uf[a] += uf[b];
        uf[b].par = a;
        return true;
    }
};

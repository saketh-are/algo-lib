template<typename V = monostate, typename E = monostate>
struct union_find {
    struct node {
        int par, rnk, size; V state;
        node(int id = 0) : par(id), rnk(0), size(1) {}
        void merge(node& o, E& e) {
            if (rnk == o.rnk) rnk++;
            size += o.size;
            state += o.state;
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

    bool unio(int a, int b, E& e = ms) {
        a = rep(a), b = rep(b);
        if (a == b) return false;
        if (uf[a].rnk < uf[b].rnk) swap(a, b);
        uf[a].merge(uf[b], e);
        uf[b].par = a;
        return true;
    }

    node& operator[](int i) { return uf[rep(i)]; }
};

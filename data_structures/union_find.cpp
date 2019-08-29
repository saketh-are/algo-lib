struct uf_monostate {
    uf_monostate(__attribute__((unused)) int id) {}
    void merge(__attribute__((unused)) uf_monostate& o,
            __attribute__((unused)) const monostate& e) {}
};

template<typename T = uf_monostate, typename E = monostate>
struct union_find {
    struct node {
        int par, rnk, size; T state;
        node(int id = 0) : par(id), rnk(0), size(1), state(id) {}
        void merge(node& o, E& e) {
            if (rnk == o.rnk) rnk++;
            if (size < o.size) swap(state, o.state);
            size += o.size;
            state.merge(o.state, e);
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

    T& state(int i) { return uf[rep(i)].state; }
};

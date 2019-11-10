template<typename V = monostate, typename I = function<V(int)>>
struct union_find {
    struct node {
        int par, rnk, size; vi elts; V state;
        node(int id = 0) : par(id), rnk(0), size(1), elts({id}) {}
        template<typename F> void merge(node& o, F proc) {
            proc(par, o.par);
            if (rnk == o.rnk) rnk++;
            size += o.size;
            if (sz(elts) < sz(o.elts)) swap(elts, o.elts);
            elts.insert(elts.end(), all(o.elts));
        }
    };

    vector<node> uf;
    union_find(int N = 0, I init = [](int i) { return V{}; }) : uf(N) {
        for (int i = 0; i < N; i++) {
            uf[i] = node(i);
            uf[i].state = init(i);
        }
    }

    int rep(int i) {
        if (i != uf[i].par)
            uf[i].par = rep(uf[i].par);
        return uf[i].par;
    }

    template<typename F = function<void(int, int)>>
    bool unio(int a, int b, F proc = [](int a, int b) {}) {
        a = rep(a), b = rep(b);
        if (a == b) return false;
        if (uf[a].rnk < uf[b].rnk) swap(a, b);
        uf[a].merge(uf[b], proc);
        uf[b].par = a;
        return true;
    }

    node& operator[](int i) { return uf[rep(i)]; }

    friend void pr(const union_find<V, I>& u) {
        pr("{"); bool f = 1;
        for (int i = 0; i < sz(u.uf); i++) if (u.uf[i].par == i) {
            if (!f) pr(", "); else f = 0;
            pr("[ ", i, " | rank=", u.uf[i].rnk, " size=", u.uf[i].size,
                    " elts=", u.uf[i].elts, " ", u.uf[i].state, "]");
        }
        pr("}");
    }
};

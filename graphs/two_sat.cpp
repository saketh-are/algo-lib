struct two_sat {
    vvi graph;
    two_sat() {}

    inline int neg(int a) { return a^1; }

    void implies(int a, int b) {
        if (a < 0 || b < 0) return;
        graph[a].pb(b);
        graph[neg(b)].pb(neg(a));
    }

    void assign(int a, int v) {
        if (v) implies(neg(a), a);
        else implies(a, neg(a));
    }
    void at_most_one(int a, int b) {
        implies(a, neg(b)); // contrapositive is automatic
    }
    void at_least_one(int a, int b) {
        implies(neg(a), b); // contrapositive is automatic
    }

    int new_var() {
        graph.pb({});
        graph.pb({});
        return sz(graph) - 1;
    }

    int make_or(int a, int b) {
        if (a < 0) return b;
        if (b < 0) return a;
        int v = new_var();
        implies(a, v);
        implies(b, v);
        return v;
    }

    int make_and(int a, int b) {
        if (a < 0) return b;
        if (b < 0) return a;
        int v = new_var();
        implies(v, a);
        implies(v, b);
        return v;
    }

    vb solve() {
        int C; vi comp; tie(C, comp) = strongly_connected_components(graph);
        vb res(sz(graph));
        for (int v = 0; v < sz(res); v++) {
            if (comp[v] == comp[neg(v)]) return {};
            res[v] = comp[v] < comp[neg(v)];
        }
        return res;
    }
};

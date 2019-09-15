struct two_sat {
    vvi graph;
    two_sat() {}

    int new_var() {
        graph.pb({});
        graph.pb({});
        return sz(graph) - 1;
    }

    void implies(int a, int b) {
        if (a < 0 || b < 0) return;
        graph[a].pb(b);
        graph[b^1].pb(a^1);
    }

    template<typename F> int nullable(int a, int b, F op) {
        return a < 0 ? b : b < 0 ? a : op(a, b);
    }

    int make_and(int a, int b) {
        return nullable(a, b, [&](int _a, int _b){
            int v = new_var();
            implies(v, _a);
            implies(v, _b);
            return v;
        });
    }

    int make_or(int a, int b) {
        return nullable(a, b, [&](int _a, int _b){
            int v = new_var();
            implies(_a, v);
            implies(_b, v);
            return v;
        });
    }

    // adds constraint that at most one of a or b can be true.
    int make_at_most_one(int a, int b) {
        return nullable(a, b, [&](int _a, int _b) {
            implies(_a, _b^1); // contrapositive is automatic
            return make_or(_a, _b);
        });
    }

    vb solve() {
        int C; vi comp; tie(C, comp) = strongly_connected_components(graph);
        vb res(sz(graph));
        for (int v = 0; v < sz(res); v++) {
            if (comp[v] == comp[v^1]) return {};
            res[v] = comp[v] < comp[v^1];
        }
        return res;
    }
};

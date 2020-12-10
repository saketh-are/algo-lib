// Problem: https://judge.yosupo.jp/problem/point_set_range_composite

// {{{ numeric/modnum.cpp }}}

// {{{ data_structures/segment_tree.cpp }}}

using mn = modnum<998244353>;

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    struct func {
        mn a, b;
        mn eval(int x) { return a * x + b; }
    };

    // g(f(x))
    auto compose = [](func f, func g) {
        return func {
            f.a * g.a,
            f.b * g.a + g.b
        };
    };

    segment_tree<func, decltype(compose)> s(N, {1, 0}, compose);

    s.assign([](int i) { int a, b; scanf("%d %d", &a, &b); return func{a, b}; });

    for (int q = 0; q < Q; q++) {
        int t;
        scanf("%d", &t);
        if (t == 0) {
            int p, c, d;
            scanf("%d %d %d", &p, &c, &d);
            s.assign(p, func{c, d});
        } else {
            int l, r, x;
            scanf("%d %d %d", &l, &r, &x);
            printf("%d\n", s.accumulate(l, r).eval(x));
        }
    }
}

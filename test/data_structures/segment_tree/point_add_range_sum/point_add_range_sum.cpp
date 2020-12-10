// Problem: https://judge.yosupo.jp/problem/point_add_range_sum

#include <functional>

// {{{ data_structures/segment_tree.cpp }}}

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    segment_tree<int64_t, std::plus<int64_t>> s(N, 0, std::plus<int64_t>());

    s.assign([](int i) { int a; scanf("%d", &a); return a; });

    for (int q = 0; q < Q; q++) {
        int t;
        scanf("%d", &t);
        if (t == 0) {
            int p, x;
            scanf("%d %d", &p, &x);
            s.assign(p, s[p] + x);
        } else {
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%ld\n", s.accumulate(l, r));
        }
    }
}

// Problem: https://judge.yosupo.jp/problem/point_add_range_sum

#include <functional>
#include <cstdio>

// {{{ data_structures/binary_indexed_tree.cpp }}}

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    binary_indexed_tree<int64_t, std::plus<int64_t>> bit(N, 0, std::plus<int64_t>());

    for (int i = 0; i < N; i++) {
        int a;
        scanf("%d", &a);
        bit.add(i, a);
    }

    for (int q = 0; q < Q; q++) {
        int t;
        scanf("%d", &t);
        if (t == 0) {
            int p, x;
            scanf("%d %d", &p, &x);
            bit.add(p, x);
        } else {
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%ld\n", bit.accumulate_prefix(r) - bit.accumulate_prefix(l));
        }
    }
}

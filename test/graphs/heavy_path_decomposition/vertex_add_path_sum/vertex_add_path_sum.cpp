#include <cstdio>
#include <vector>

// {{{ graphs/heavy_path_decomposition.cpp }}}
// {{{ data_structures/binary_indexed_tree.cpp }}}

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    std::vector<int> init(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &init[i]);

    tree<edge> tr(N, 0);
    re(tr, EDGE_LIST, 0);

    heavy_path_decomposition<edge> hld(tr);

    binary_indexed_tree<int64_t, std::plus<int64_t>> bit(N, 0, std::plus<int64_t>());

    for (int i = 0; i < N; i++)
        bit.add(hld.index(i), init[i]);

    for (int q = 0; q < Q; q++) {
        int t;
        scanf("%d", &t);

        if (t == 0) {
            int p, x;
            scanf("%d %d", &p, &x);
            bit.add(hld.index(p), x);
        } else {
            int u, v;
            scanf("%d %d", &u, &v);
            printf("%jd\n", hld.accumulate_commutative(u, v, true, int64_t(0), std::plus<int64_t>(),
                [&bit](uint32_t i, uint32_t j) { return bit.accumulate_prefix(j) - bit.accumulate_prefix(i); }));
        }
    }

    return 0;
}


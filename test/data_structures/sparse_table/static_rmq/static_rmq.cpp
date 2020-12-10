#include <cstdio>

// {{{ data_structures/sparse_table.cpp }}}

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    std::vector<int> a(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i]);

    sparse_table<int> st(N, std::min<int>, [&](int i) { return a[i]; });

    for (int q = 0; q < Q; q++) {
        int l, r;
        scanf("%d %d", &l, &r);

        printf("%d\n", st.accumulate(l, r));
    }

    return 0;
}

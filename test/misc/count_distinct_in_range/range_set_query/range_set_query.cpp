// Problem: https://atcoder.jp/contests/abc174/tasks/abc174_f

#include <cstdio>
#include <vector>
#include <iostream>

// {{{ misc/count_distinct_in_range.cpp }}}

int main() {
    int N, Q;
    scanf("%d %d", &N, &Q);

    std::vector<int> colors(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &colors[i]);

    count_distinct_in_range<int> cd(colors.begin(), colors.end());

    for (int q = 0; q < Q; q++) {
        int l, r;
        scanf("%d %d", &l, &r);
        printf("%d\n", cd.get_count(l - 1, r));
    }

    return 0;
}


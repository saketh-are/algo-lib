// Problem: https://codeforces.com/contest/813/problem/E

#include <cstdio>
#include <vector>

// {{{ misc/count_distinct_in_range }}}

int main() {
    int N, K;
    scanf("%d %d", &N, &K);

    std::vector<int> a(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i]);

    count_distinct_in_range<int> cd(a.begin(), a.end(), K);

    int Q, last = 0;
    scanf("%d", &Q);

    for (int q = 0; q < Q; q++) {
        int x, y;
        scanf("%d %d", &x, &y);

        int l = (x + last) % N;
        int r = (y + last) % N;
        if (l > r) std::swap(l, r);

        printf("%d\n", last = cd.get_count(l, r + 1));
    }

    return 0;
}


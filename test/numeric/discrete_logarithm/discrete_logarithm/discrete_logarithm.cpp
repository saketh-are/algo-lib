// Problem: https://judge.yosupo.jp/problem/discrete_logarithm_mod

#include <cstdio>

// {{{ numeric/discrete_logarithm.cpp }}}

int main() {
    int T;
    scanf("%d", &T);

    for (int t = 0; t < T; t++) {
        int x, y, m;
        scanf("%d %d %d", &x, &y, &m);

        int ans = discrete_log(x, y, m);

        printf("%d\n", ans);
    }
}

// Problem: https://judge.yosupo.jp/problem/sqrt_mod

#include <cstdio>

// {{{ numeric/tonelli-shanks.cpp }}}

int main() {
    int T;
    scanf("%d", &T);

    for (int t = 0; t < T; t++) {
        int Y, P;
        scanf("%d %d", &Y, &P);

        printf("%d\n", sqrt(Y, P));
    }
}

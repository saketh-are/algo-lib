// Problem: https://judge.yosupo.jp/problem/convolution_mod

#include <vector>
using namespace std;

// {{{ numeric/ntt.cpp }}}
using namespace ntt;

using mn = modnum<998244353>;

int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    vector<mn> a(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i].v);

    vector<mn> b(M);
    for (int i = 0; i < M; i++)
        scanf("%d", &b[i].v);

    vector<mn> c = a * b;
    for (int i = 0; i < int(c.size()); i++) {
        if (i) printf(" ");
        printf("%d", int(c[i]));
    }
    printf("\n");
}

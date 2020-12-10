// Problem: https://judge.yosupo.jp/problem/convolution_mod_1000000007

#include <vector>
using namespace std;

// {{{ numeric/complex_fft.cpp }}}
using namespace complex_fft;

using mn = modnum<int(1e9 + 7)>;

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

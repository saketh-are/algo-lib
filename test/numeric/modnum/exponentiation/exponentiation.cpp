// https://cses.fi/problemset/task/1712

// {{{ numeric/modnum.cpp }}}

using mn     = modnum<int(1e9 + 7)>;
using mn_exp = modnum<mn::totient()>;

int main() {
    int N;
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);

        mn_exp e = mn_exp(b).pow(c);
        mn ans = mn(a).pow(int(e));

        printf("%d\n", int(ans));
    }
}

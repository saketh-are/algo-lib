/*
 * Garner's algorithm: finds the unique v modulo prod(MOD)
 * such that v = n[i] (MOD[i]) for all i.
 * Elements of MOD should be pairwise co-prime.
 */
int crt(const vi& MOD, const vi& n) {
    const int M = sz(MOD);

    vi x(M);
    for (int i = 0; i < M; i++) {
        x[i] = n[i];
        for (int j = 0; j < i; j++) {
            x[i] = ll(x[i] - x[j]) * inverse(MOD[i], MOD[j] % MOD[i]) % MOD[i];
            if (x[i] < 0) x[i] += MOD[i];
        }
    }

    int v = 0;
    for (int i = M - 1; i >= 0; i--)
        v = MOD[i] * v + x[i];
    return v;
}

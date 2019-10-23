/*
 * Garner's algorithm: finds the unique v modulo prod(MODS)
 * such that v is congruent to n[i] modulo MODS[i] for all i.
 * Elements of MODS should be pairwise co-prime.
 */
ll crt(const vi& MODS, const vi& n) {
    auto inverse = [](int M, int n) {
        int g = M, x = 0, y = 1;
        for (int r = n; r != 0; ) {
            int q = g / r;
            g %= r; swap(g, r);
            x -= q * y; swap(x, y);
        }

        assert(g == 1);
        assert(y == M || y == -M);
        return x < 0 ? x + M : x;
    };

    const int M = sz(MODS);

    vi x(M);
    for (int i = 0; i < M; i++) {
        x[i] = n[i];
        for (int j = 0; j < i; j++) {
            x[i] = ll(x[i] - x[j]) * inverse(MODS[i], MODS[j] % MODS[i]) % MODS[i];
            if (x[i] < 0) x[i] += MODS[i];
        }
    }

    ll v = 0;
    for (int i = M - 1; i >= 0; i--)
        v = MODS[i] * v + x[i];
    return v;
}

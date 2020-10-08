/*
 * Garner's algorithm: finds the unique v modulo prod(MODS)
 * such that v is congruent to n[i] modulo MODS[i] for all i.
 * Elements of MODS should be pairwise co-prime.
 */

template<typename V> V crt(const vector<V> MODS, const vector<V> n) {
    auto inverse = [](V M, V n) {
        V g = M, x = 0, y = 1;
        for (V r = n; r != 0; ) {
            V q = g / r;
            g %= r; swap(g, r);
            x -= q * y; swap(x, y);
        }

        assert(g == 1);
        assert(y == M || y == -M);
        return x < 0 ? x + M : x;
    };

    const int M = sz(MODS);

    vector<V> x(M);
    for (int i = 0; i < M; i++) {
        x[i] = n[i];
        for (int j = 0; j < i; j++) {
            x[i] = V(x[i] - x[j]) * inverse(MODS[i], MODS[j] % MODS[i]) % MODS[i];
            if (x[i] < 0) x[i] += MODS[i];
        }
    }

    V v = 0;
    for (int i = M - 1; i >= 0; i--)
        v = MODS[i] * v + x[i];
    return v;
}

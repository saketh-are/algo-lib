int mod_pow(int MOD, int v, int e) {
    if (e < 0) return mod_pow(MOD, inverse(MOD, v), -e);
    if (e == 0) return 1;
    if (e&1) return ll(v) * mod_pow(MOD, v, e - 1) % MOD;
    return mod_pow(MOD, ll(v) * v % MOD, e/2);
}

/*
 * Tonelli–Shanks algorithm: finds r such that r*r = n (MOD).
 * MOD should be a prime power.
 * n should be a quadratic residue modulo MOD.
 */
int sqrt(int MOD, int n) {
    assert(0 <= n && n < MOD);
    if (n == 0) return 0;
    if (MOD == 2) return n;
    if (mod_pow(MOD, n, (MOD - 1) >> 1) != 1) return -1;

    int Q = MOD - 1, S = 0;
    while (!(Q&1)) Q >>= 1, S++;

    int z = 2;
    while (mod_pow(MOD, z, (MOD - 1) >> 1) == 1) z++;
    z = mod_pow(MOD, z, Q);
    assert(mod_pow(MOD, z, 1 << (S - 1)) == MOD - 1);

    int r = mod_pow(MOD, n, (Q + 1) >> 1),
        t = mod_pow(MOD, n, Q);

    for (int m = S - 2; t != 1; m--) {
        int nz = ll(z) * z % MOD;
        if (mod_pow(MOD, t, 1 << m) != 1) {
            r = ll(r) * z % MOD;
            t = ll(t) * nz % MOD;
        }
        z = nz;
    }

    assert(ll(r) * r % MOD == n);
    return r;
}

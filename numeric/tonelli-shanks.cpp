#include <cassert>
#include <cstdint>

int product(int x, int y, int MOD) {
    return int(int64_t(x) * y % MOD);
}

int mod_pow(int x, int k, int MOD) {
    if (!k) return 1;
    if (k&1) return product(x, mod_pow(x, k - 1, MOD), MOD);
    return mod_pow(product(x, x, MOD), k / 2, MOD);
}

/*
 * Tonelli–Shanks algorithm: finds some r such that r*r = n (MOD).
 * MOD should be a prime power.
 * Returns -1 if no such r exists.
 */
int sqrt(int n, int MOD) {
    assert(0 <= n && n < MOD);

    if (n == 0) return 0;
    if (MOD == 2) return n;
    if (mod_pow(n, (MOD - 1) >> 1, MOD) != 1) return -1;

    int Q = MOD - 1, S = 0;
    while (!(Q&1)) Q >>= 1, S++;

    int z = 2;
    while (mod_pow(z, (MOD - 1) >> 1, MOD) == 1) z++;
    z = mod_pow(z, Q, MOD);
    assert(mod_pow(z, 1 << (S - 1), MOD) == MOD - 1);

    int r = mod_pow(n, (Q + 1) >> 1, MOD),
        t = mod_pow(n, Q, MOD);

    for (int m = S - 2; t != 1; m--) {
        int nz = product(z, z, MOD);
        if (mod_pow(t, 1 << m, MOD) != 1) {
            r = product(r, z,  MOD);
            t = product(t, nz, MOD);
        }
        z = nz;
    }

    assert(product(r, r, MOD) == n);
    return r;
}

// {{{ data_structures/splitmix64_hash_map.cpp }}}

#include <cstdint>
#include <numeric>

int product(int x, int y, int MOD) {
    return int(int64_t(x) * y % MOD);
}

int mod_pow(int x, int k, int MOD) {
    if (!k) return 1;
    if (k&1) return product(x, mod_pow(x, k - 1, MOD), MOD);
    return mod_pow(product(x, x, MOD), k / 2, MOD);
}

int totient(int v) {
    int tot = v;
    for (int p = 2; p * p <= v; p++) if (v % p == 0) {
        tot = tot / p * (p - 1);
        while (v % p == 0) v /= p;
    }
    if (v > 1) tot = tot / v * (v - 1);
    return tot;
}

/* Returns the smallest K >= 0 such that init * pow(x, K) === y modulo MOD.
 * Returns -1 if no such K exists. Runs in O(sqrt(MOD)).
 */
int discrete_log(int x, int y, int MOD, int init = 1) {
    if (x == 0)
        return y == 1 ? 0 : y == 0 ? MOD > 1 : -1;

    int prefix = 0;
    while (init != y && std::gcd(init, MOD) != std::gcd(product(init, x, MOD), MOD)) {
        init = product(init, x, MOD);
        prefix++;
    }

    if (init == y)
        return prefix;

    if (std::gcd(init, MOD) != std::gcd(y, MOD))
        return -1;

    MOD = MOD / std::gcd(init, MOD);

    x %= MOD;
    y %= MOD;
    init %= MOD;

    int subgroup_order = totient(MOD);

    y = product(y, mod_pow(init, subgroup_order - 1, MOD), MOD);

    int step_size = 0;
    while (step_size * step_size < subgroup_order)
        step_size++;

    umap<int, int> table;

    int baby_step = 1;
    for (int i = 0; i < step_size; i++) {
        table[baby_step] = i;
        baby_step = product(baby_step, x, MOD);
    }

    int giant_step = mod_pow(x, subgroup_order - step_size, MOD);
    for (int i = 0; i < step_size; i++) {
        auto it = table.find(y);
        if (it != table.end())
            return prefix + i * step_size + it->second;
        y = product(y, giant_step, MOD);
    }

    return -1;
}

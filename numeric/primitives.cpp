namespace number_theory {
    template<size_t N> struct sieve {
        vi primes, least_prime, largest_proper, phi, mu;
        sieve() : least_prime(N + 1), largest_proper(N + 1), phi(N + 1), mu(N + 1) {
            for (int i = 2; i <= N; i++) {
                if (!least_prime[i]) {
                    least_prime[i] = i;
                    largest_proper[i] = 1;
                    primes.push_back(i);
                }
                for (int x : primes) {
                    if (x > least_prime[i] || i * x > N) break;
                    least_prime[i * x] = x;
                    largest_proper[i * x] = i;
                }
            }

            phi[1] = mu[1] = 1;
            for (int i = 2; i <= N; i++) {
                if (least_prime[largest_proper[i]] == least_prime[i]) {
                    phi[i] = phi[largest_proper[i]] * least_prime[i];
                    mu[i] = 0;
                } else {
                    phi[i] = phi[largest_proper[i]] * (least_prime[i] - 1);
                    mu[i] = -mu[largest_proper[i]];
                }
            }
        }

        bool is_prime(int v) {
            assert(0 < v && v < N);
            return least_prime[v] == v;
        }

        vpii factor(int v) {
            assert(0 < v && v <= N);
            vpii res;
            while (v > 1) {
                res.emplace_back(least_prime[v], 0);
                while (least_prime[v] == res.back().f) {
                    v = largest_proper[v];
                    res.back().s++;
                }
            }
            reverse(all(res));
            return res;
        }

        vi divisors(int v) {
            if (v == 1) return {1};

            int p = least_prime[v], m = 0;
            while (v % p == 0) v /= p, m++;

            vi res = divisors(v), fin;
            for (int d : res)
                for (int pc = 0, t = d; pc <= m; pc++, t *= p)
                    fin.pb(t);
            return fin;
        }
    };

    vi factor(int V) {
        vi res;
        for (int p = 2; p * p <= V; p++) {
            while (V % p == 0) {
                res.push_back(p);
                V /= p;
            }
        }
        if (V > 1) res.push_back(V);
        return res;
    }

    int inverse(int MOD, int v) {
        int g = MOD, x = 0, y = 1;
        for (int r = v; r != 0; ) {
            int q = g / r;
            g %= r; swap(g, r);
            x -= q * y; swap(x, y);
        }

        assert(g == 1);
        assert(y == MOD || y == -MOD);
        return x < 0 ? x + MOD : x;
    }

    int mod_pow(int MOD, int v, int e) {
        if (e < 0) return mod_pow(MOD, inverse(MOD, v), -e);
        if (e == 0) return 1;
        if (e&1) return ll(v) * mod_pow(MOD, v, e - 1) % MOD;
        return mod_pow(MOD, ll(v) * v % MOD, e/2);
    }

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
}

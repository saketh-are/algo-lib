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

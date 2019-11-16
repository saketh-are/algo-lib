template<size_t N> struct sieve {
    vi primes;
    struct num {
        int lp;   // least prime divisor
        char lpk; // multiplicity of least prime divisor
        int pp;   // lp ** lpk
        int pd;   // largest proper divisor (num / lp)
        int wlp;  // largest divisor without the least prime (num / (lp ** lpk))
        int phi;  // euler's totient function
        char mu;  // mobius function
    };
    vector<num> nums;
    const num& operator[](int i){ return nums[i]; }

    sieve() : nums(N + 1) {
        nums[1] = { -1, 0, 1, -1, -1, 1, 1 };
        for (int i = 2; i <= N; i++) {
            num& n = nums[i];
            if (!n.lp) {
                n = { i, 1, i, 1, 1, i-1, -1 };
                primes.pb(i);
            }
            for (int p : primes) {
                if (p > n.lp || p * i > N) break;
                if (p < n.lp) nums[p * i] = { p, 1, p, i, i, n.phi * (p - 1), -n.mu };
                else nums[p * i] = { p, n.lpk + 1, n.pp * p, i, n.wlp, n.phi * p, 0 };
            }
        }
    }

    bool is_prime(int v) {
        assert(0 < v && v <= N);
        return nums[v].lp == v;
    }

    const vpii& factor(int v) {
        assert(0 < v && v <= N);
        static vpii res; res.clear();
        for (; v > 1; v = nums[v].wlp)
            res.emplace_back(nums[v].lp, nums[v].lpk);
        reverse(all(res));
        return res;
    }

    vi divisors(int v) {
        assert(0 < v && v <= N);
        if (v == 1) return {1};
        vi res;
        for (int d : divisors(nums[v].wlp))
            for (int k = 0; k <= nums[v].lpk; k++, d *= nums[v].lp)
                res.pb(d);
        return res;
    }

    int gcd(int a, int b) {
        if (a == 0) return b; else assert(0 < a && a <= N);
        if (b == 0) return a; else assert(0 < b && b <= N);
        int g = 1;
        while (a > 1 && b > 1) {
            const num &na = nums[a], &nb = nums[b];
            if (na.lp == nb.lp) {
                g *= min(na.pp, nb.pp);
                a = na.wlp, b = nb.wlp;
            } else if (na.lp < nb.lp) a = na.wlp;
            else b = nb.wlp;
        }
        return g;
    }
};

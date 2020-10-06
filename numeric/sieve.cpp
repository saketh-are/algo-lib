enum DIVISOR_TYPE { ALL, SQUARE_FREE };
template<int MAXV> struct sieve {
    vi primes;
    struct num {
        int  least_prime;       // least prime divisor
        int  div_least_prime;   // num divided by least_prime
        char lp_multiplicity;   // multiplicity of the least prime divisor
        char mu;                // mobius function
        int  phi;               // euler's totient function

        static num ONE() { return { INT_MAX, 1, 0, 1, 1}; }

        num prod(int my_value, int p) const {
            if (p < least_prime)
                return { p, my_value, 1, char(-mu), phi * (p - 1) };
            assert(p == least_prime);
            return { p, my_value, char(lp_multiplicity + 1), 0, phi * p };
        }
    };
    vector<num> nums;
    const num& operator[](int i){ return nums[i]; }

    sieve() : nums(MAXV) {
        nums[1] = num::ONE();
        for (int v = 2; v < MAXV; v++) {
            num& n = nums[v];
            if (!n.least_prime) {
                n = nums[1].prod(1, v);
                primes.pb(v);
            }
            for (int p : primes) {
                if (p > n.least_prime || v * p >= MAXV) break;
                nums[v * p] = n.prod(v, p);
            }
        }
    }

    bool is_prime(int v) const {
        assert(0 < v && v < MAXV);
        return nums[v].least_prime == v;
    }

    int eliminate_least_prime(int v) const {
        assert(1 < v && v < MAXV);
        for (int m = nums[v].lp_multiplicity; m > 0; m--)
            v = nums[v].div_least_prime;
        return v;
    }

    const vpii& factor(int v) const {
        assert(0 < v && v < MAXV);
        static vpii res; res.clear();
        for (; v > 1; v = eliminate_least_prime(v))
            res.emplace_back(nums[v].least_prime, nums[v].lp_multiplicity);
        reverse(all(res));
        return res;
    }

    template<typename F> void for_each_divisor_unordered(int v, F f,
            DIVISOR_TYPE t = ALL, int d = 1) const {
        assert(0 < v && v < MAXV);
        if (v == 1) { f(d); return; }
        int w = eliminate_least_prime(v);
        char M = min(nums[v].lp_multiplicity, char(t == ALL ? CHAR_MAX : 1));
        for (int m = 0; m <= M; m++, d *= nums[v].least_prime) {
            for_each_divisor_unordered(w, f, t, d);
        }
    }

    const vi& unordered_divisors(int v, DIVISOR_TYPE t = ALL) const {
        assert(0 < v && v < MAXV);
        static vi res; res.clear();
        for_each_divisor_unordered(v, [&](int d) { res.pb(d); }, t);
        return res;
    }
}

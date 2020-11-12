struct bignum_addpow2_compare {
    /* 'less_than' relies on prefix hashes to find the most significant bit at which
     * two numbers differ. It can falsely claim x !< y when a collision occurs.
     * Increasing HASH_COUNT decreases the probability of error while slowing all operations
     * and increasing memory usage.
     */
    static const int HASH_COUNT = 3;
    using P = polynomial_hash<modnum<int(1e9 + 7)>, HASH_COUNT, 2>;

    struct fingerprint {
        P hash;
        int trailing_ones;

        fingerprint (bool b) : hash(b), trailing_ones(b) { }
        fingerprint (P hash_, int trailing_ones_) : hash(hash_), trailing_ones(trailing_ones_) { }

        int bit_width() const { return hash.N; }
        fingerprint operator+(const fingerprint &o) const {
            return fingerprint {
                hash + o.hash,
                (o.trailing_ones == o.bit_width() ? trailing_ones : 0) + o.trailing_ones
            };
        }
    };

    struct num {
        fingerprint v;
        int left = -1, right = -1;
    };

    vector<num> pool = { num{fingerprint(0)}, num{fingerprint(1)} };
    vi zero = { 0 };
    int one = 1;

    int concat(int x, int y) {
        pool.push_back({ pool[x].v + pool[y].v, x, y });
        return int(pool.size()) - 1;
    }

    int get_zero_of_width(int bit_width) {
        int index = __builtin_ctz(bit_width);
        while (index >= int(zero.size()))
            zero.push_back(concat(zero.back(), zero.back()));
        return zero[index];
    }

    int carry_count(int x, int pow) const {
        const int BITS = pool[x].v.bit_width();

        if (BITS <= pow) return 0;
        if (BITS == 1) return pool[x].v.trailing_ones;
        if (BITS == pool[x].v.trailing_ones) return BITS - pow;

        int res = carry_count(pool[x].right, pow);
        if (BITS / 2 <= pow + res)
            res += carry_count(pool[x].left, pow + res - BITS / 2);
        return res;
    }

    int invert_range(int x, int L, int R) {
        const int BITS = pool[x].v.bit_width();
        if (BITS == 1)
            return x^1;
        if (L == 0 && BITS == R && pool[x].v.trailing_ones == BITS)
            return get_zero_of_width(BITS);

        int left  = R > BITS / 2 ? invert_range(pool[x].left, max(0, L - BITS / 2), R - BITS / 2) : pool[x].left;
        int right = L < BITS / 2 ? invert_range(pool[x].right, L, min(R, BITS / 2)) : pool[x].right;
        return concat(left, right);
    }

    int add_pow2(int x, int pow) {
        int carries = carry_count(x, pow);
        while (pool[x].v.bit_width() <= pow + carries)
            x = concat(get_zero_of_width(pool[x].v.bit_width()), x);
        return invert_range(x, pow, pow + carries + 1);
    }

    bool less_than(int x, int y) const {
        if (pool[x].v.bit_width() != pool[y].v.bit_width())
            return pool[x].v.bit_width() < pool[y].v.bit_width();
        if (pool[x].v.bit_width() == 1)
            return pool[x].v.trailing_ones < pool[y].v.trailing_ones;
        if (pool[pool[x].left].v.hash == pool[pool[y].left].v.hash)
            return less_than(pool[x].right, pool[y].right);
        else
            return less_than(pool[x].left, pool[y].left);
    }

    void print_bits(int x) const {
        if (pool[x].v.bit_width() == 1) pr(pool[x].v.trailing_ones);
        else print_bits(pool[x].left), print_bits(pool[x].right);
    }
};

template<typename T, typename U, bool PROP = true> struct segment_tree_lazy {
    using TT = function<T(const T&, const T&)>;
    using UU = function<U(const U&, const U&)>;
    using UT = function<T(const U&, const T&)>;

    size_t SZ;
    T tid; TT tt; UU uu; UT ut;
    vector<T> table;
    vb has; vector<U> ops;

    segment_tree_lazy() {}
    segment_tree_lazy(int _SZ, T _tid, TT _tt, UU _uu, UT _ut) :
            SZ(_SZ), tid(_tid), tt(_tt), uu(_uu), ut(_ut) {
        table.resize(2 * SZ, tid), has.resize(SZ), ops.resize(SZ);
    }
    template<typename L> void set_leaves(L create) {
        F0R (i, SZ) table[SZ + i] = create(i);
        FORd (i, 1, SZ) table[i] = tt(table[2 * i], table[2 * i + 1]);
    }

    void apply(int i, const U &op) {
        table[i] = ut(op, table[i]);
        if (i < SZ) has[i] = true, ops[i] = uu(op, ops[i]);
    }
    void rebuild(int i) {
        for (i /= 2; i; i /= 2)
            table[i] = ut(ops[i], tt(table[2 * i], table[2 * i + 1]));
    }
    void propagate(int i) {
        for (int j = 31 - __builtin_clz(i); j > 0; j--) {
            if (int k = i >> j; has[k]) {
                apply(2 * k, ops[k]);
                apply(2 * k + 1, ops[k]);
                has[k] = false, ops[k] = U{};
            }
        }
    }

    void replace(int i, T v) {
        i += SZ;
        if (PROP) propagate(i);
        table[i] = v;
        rebuild(i);
    }

    void operator()(int i, int j, U op) {
        i += SZ, j += SZ;
        if (PROP) propagate(i), propagate(j - 1);
        for (int l = i, r = j; l < r; l /= 2, r /= 2) {
            if (l&1) apply(l++, op);
            if (r&1) apply(--r, op);
        }
        rebuild(i), rebuild(j - 1);
    }

    T operator()(int i, int j) {
        i += SZ, j += SZ;
        if (PROP) propagate(i), propagate(j - 1);
        T left = tid, right = tid;
        for (; i < j; i /= 2, j /= 2) {
            if (i&1) left = tt(left, table[i++]);
            if (j&1) right = tt(table[--j], right);
        }
        return tt(left, right);
    }
    T operator[](int i) { return (*this)(i, i+1); }
};

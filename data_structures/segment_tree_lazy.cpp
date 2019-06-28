template<typename T, typename U, bool PROP = true> struct segment_tree_lazy {
    int S;
    vector<T> table;
    vb has; vector<U> ops;

    segment_tree_lazy(int _S) : S(_S) {
        table.resize(2 * S), has.resize(S), ops.resize(S);
    }

    segment_tree_lazy(auto begin, auto end) : segment_tree_lazy(distance(begin, end)) {
        copy(begin, end, table.begin() + S);
        for (int i = S - 1; i > 0; i--)
            table[i] = table[2 * i] + table[2 * i + 1];
    }

    void apply(int i, const U &op) {
        table[i] = op(table[i]);
        if (i < S) has[i] = true, ops[i] = op(ops[i]);
    }

    void rebuild(int i) {
        for (i /= 2; i; i /= 2)
            table[i] = ops[i](table[2 * i] + table[2 * i + 1]);
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
        if (PROP) propagate(i + S);
        table[i + S] = v;
        rebuild(i + S);
    }

    void operator()(int i, int j, U op) {
        i += S, j += S;
        if (PROP) propagate(i), propagate(j - 1);
        for (int l = i, r = j; l < r; l /= 2, r /= 2) {
            if (l&1) apply(l++, op);
            if (r&1) apply(--r, op);
        }
        rebuild(i), rebuild(j - 1);
    }

    T operator()(int i, int j) {
        i += S, j += S;
        if (PROP) propagate(i), propagate(j - 1);
        T left{}, right{};
        for (; i < j; i /= 2, j /= 2) {
            if (i&1) left = left + table[i++];
            if (j&1) right = table[--j] + right;
        }
        return left + right;
    }
    T operator()(int i) { return *this(i, i+1); }
};

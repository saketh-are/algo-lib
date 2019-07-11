/*
 * Supports O(1) range queries on an immutable array of items.
 * Requires that type T's combine operator is associate, commutative,
 * and idempotent.
 */
template<typename T> struct sparse_table {
    int S, L;
    vector<T> table;
    T& entry(int l, int i) { return table[l * S + i]; }
    const T& entry(int l, int i) const { return table[l * S + i]; }

    sparse_table() {}
    sparse_table(const vector<T>& elts) : S(sz(elts)) {
        L = 32 - __builtin_clz(max(S - 1, 1));
        table.resize(L * S);
        copy(all(elts), table.begin());

        for (int l = 0; l + 1 < L; l++) {
            for (int i = 0; i < S; i++) {
                entry(l + 1, i) = entry(l, i);
                if (i + (1 << l) < S)
                    entry(l + 1, i) = entry(l + 1, i) + entry(l, i + (1 << l));
            }
        }
    }

    // Accumulates the elements at indices in [i, j) in O(1)
    T operator()(int i, int j) const {
        if (i >= j) return T{};
        int l = j - i - 1 ? 31 - __builtin_clz(j - i - 1) : 0;
        return entry(l, i) + entry(l, j - (1 << l));
    }
};

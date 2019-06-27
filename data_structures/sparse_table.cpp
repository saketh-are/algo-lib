/*
 * Supports O(1) range queries on an immutable array of items.
 * Requires that type T's combine operator is associate, commutative,
 * and idempotent.
 */
template<typename T> struct sparse_table {
    int S, L;
    vector<T> table;
    T& entry(int l, int i) { return table[l * S + i]; }

    // Constructs the table in O(S log S) time and memory
    sparse_table(const vector<T>& elts = {}) : S(sz(elts)) {
        L = S ? 32 - __builtin_clz(S) : 0;
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

    // Returns elts[i] + ... + elts[j] in O(1)
    T query(int i, int j) {
        int l = 31 - __builtin_clz(j - i + 1);
        return entry(l, i) + entry(l, j - (1 << l) + 1);
    }
};

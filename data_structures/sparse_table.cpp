template<typename T, typename F> struct sparse_table {
    int SZ;
    F tt; // idempotent
    vector<vector<T>> table;

    sparse_table() : SZ(0) {}
    template<typename I> sparse_table(int SZ_, F tt_, I init) : SZ(SZ_), tt(tt_) {
        const int L = 32 - __builtin_clz(max(SZ - 1, 1));
        table.resz(L, vector<T>(SZ));

        for (int i = 0; i < SZ; i++) table[0][i] = init(i);

        for (int l = 0; l + 1 < L; l++)
            for (int i = 0; i + (2 << l) <= SZ; i++)
                table[l+1][i] = tt(table[l][i], table[l][i + (1 << l)]);
    }

    // Accumulates the elements at indices in [i, j) in O(1)
    T operator()(int i, int j) const {
        assert(0 <= i && i < j && j <= SZ);
        int l = j - i - 1 ? 31 - __builtin_clz(j - i - 1) : 0;
        return tt(table[l][i], table[l][j - (1 << l)]);
    }
};

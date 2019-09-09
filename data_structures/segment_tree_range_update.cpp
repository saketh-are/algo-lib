template<typename T, typename F> struct segment_tree {
    size_t SZ;
    T id; F tt;
    vector<T> table;

    segment_tree() {}
    segment_tree(size_t _SZ, T _id, F _tt) : SZ(_SZ), id(_id), tt(_tt) {
        table.resize(2 * SZ, id);
    }

    // Adds v to the indices in [i, j)
    void operator()(int i, int j, T v) {
        for (i += SZ, j += SZ; i < j; i /= 2, j /= 2) {
            if (i&1) table[i] = tt(table[i], v), i++;
            if (j&1) --j, table[j] = tt(table[j], v);
        }
    }

    // Computes the value at index i
    const T operator()(int i) const {
        T res = id;
        for (i += SZ; i; i /= 2)
            res = tt(res, table[i]);
        return res;
    }
};

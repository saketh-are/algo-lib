template<typename T> struct segment_tree {
    using F = function<T(const T&, const T&)>;

    size_t SZ;
    T id;
    F combine;
    vector<T> table;

    segment_tree() {}
    segment_tree(size_t _SZ, T _id, F _combine) : SZ(_SZ), id(_id), combine(_combine) {
        table.resize(2 * SZ);
    }

    template<typename L> void set_leaves(L create) {
        F0R (i, SZ) table[SZ + i] = create(i);
        FORd (i, 1, SZ) table[i] = combine(table[2 * i], table[2 * i + 1]);
    }

    // Replaces the element at index i with v
    void replace(int i, T v) {
        table[i += SZ] = v;
        for (i /= 2; i > 0; i /= 2) {
            table[i] = combine(table[2 * i], table[2 * i + 1]);
        }
    }

    // Accumulates the elements at indices in [i, j)
    T operator()(int i, int j) const {
        T left = id, right = id;
        for (i += SZ, j += SZ; i < j; i /= 2, j /= 2) {
            if (i&1) left = combine(left, table[i++]);
            if (j&1) right = combine(table[--j], right);
        }
        return combine(left, right);
    }
    const T& operator[](int i) const { return table[SZ + i]; }
};

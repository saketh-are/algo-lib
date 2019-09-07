template<typename T, typename F> struct segment_tree {
    int SZ;
    T id; F tt;
    vector<T> table;

    segment_tree() {}
    segment_tree(int _SZ, T _id, F _tt) : SZ(_SZ), id(_id), tt(_tt) {
        table.resize(2 * SZ);
    }
    template<typename L> void set_leaves(L create) {
        F0R (i, SZ) table[SZ + i] = create(i);
        FORd (i, 1, SZ) table[i] = tt(table[2 * i], table[2 * i + 1]);
    }

    // Replaces the element at index i with v
    void replace(int i, T v) {
        table[i += SZ] = v;
        for (i /= 2; i > 0; i /= 2) {
            table[i] = tt(table[2 * i], table[2 * i + 1]);
        }
    }

    // Accumulates the elements at indices in [i, j)
    T operator()(int i, int j) const {
        T left = id, right = id;
        for (i += SZ, j += SZ; i < j; i /= 2, j /= 2) {
            if (i&1) left = tt(left, table[i++]);
            if (j&1) right = tt(table[--j], right);
        }
        return tt(left, right);
    }
    const T& operator[](int i) const { return table[SZ + i]; }
};

template<typename T, typename F> struct full_segment_tree : segment_tree<T, F> {
    // Internal size is rounded up so that all layers are full
    full_segment_tree() {}
    full_segment_tree(int _SZ, T _id, F _tt) :
        segment_tree<T, F>(1 << (32 - __builtin_clz(_SZ - 1)), _id, _tt) {}

    /*
     * Returns the first i in [0, S] such that comp(this(0, i))
     * evaluates to true. Returns -1 if no such i exists.
     * Requires that comp(this(0, i)) is non-decreasing in i.
     */
    template<typename C> int lower_bound(C comp) const {
        if (!comp(this->table[1])) return -1;
        if (comp(T{})) return 0;

        int loc = 1;
        for (T cur{}; loc < this->SZ; ) {
            T mid = this->tt(cur, this->table[2 * loc]);
            if (comp(mid)) {
                loc = 2 * loc;
            } else {
                cur = mid;
                loc = 2 * loc + 1;
            }
        }
        return loc - this->SZ + 1;
    }
};

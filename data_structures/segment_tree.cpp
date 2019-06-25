/*
 * Supports range sum queries on a mutable array of Ts.
 * Requires that T's + operator is associative and that T() is the
 * additive identity: T() + t = t = t + T() for any T t.
 */
template<typename T> struct segment_tree {
    int S;
    vector<T> table;

    segment_tree<T>(int _S) : S(_S) {
        table.resize(2 * S);
    }

    segment_tree<T>(auto begin, auto end) : segment_tree(distance(begin, end)) {
        copy(begin, end, table.begin() + S);
        for (int i = S - 1; i > 0; i--)
            table[i] = table[2 * i] + table[2 * i + 1];
    }

    // Replaces the element at index i with v
    void replace(int i, T v) {
        table[i += S] = v;
        for (i /= 2; i > 0; i /= 2) {
            table[i] = table[2 * i] + table[2 * i + 1];
        }
    }

    // Returns the value of the element at index i
    const T& operator()(int i) const { return table[S + i]; }

    // Returns the sum of the elements at indices in [i, j)
    T operator()(int i, int j) const {
        T left{}, right{};
        for (i += S, j += S; i < j; i /= 2, j /= 2) {
            if (i&1) left = left + table[i++];
            if (j&1) right = table[--j] + right;
        }
        return left + right;
    }
};

template<typename T> struct full_segment_tree : segment_tree<T> {
    // Internal size is rounded up so that all layers are full
    full_segment_tree<T>(int _S) : segment_tree<T>(1 << (32 - __builtin_clz(_S - 1))) {}

    /*
     * Returns the first i in [0, S] such that comp(this(0, i))
     * evaluates to true. Returns -1 if no such i exists.
     * Requires that comp(this(0, i)) is non-decreasing in i.
     */
    int lower_bound(const auto& comp) const {
        if (!comp(this->table[1])) return -1;
        if (comp(T{})) return 0;

        int loc = 1;
        for (T cur{}; loc < this->S; ) {
            T mid = cur + this->table[2 * loc];
            if (comp(mid)) {
                loc = 2 * loc;
            } else {
                cur = mid;
                loc = 2 * loc + 1;
            }
        }
        return loc - this->S + 1;
    }
};

/*
 * Supports O(logS) range queries and updates on an array of items.
 * Requires that type T's combine operator is associative and
 * that T() + t = t + T() = t for all T t.
 */
template<typename T> struct seg_tree {
    int S;
    vector<T> value;

    seg_tree<T>(int S) : S(S) {
        value.resize(2 * S);
    }

    // Rebuilds all non-leaf layers of the tree in O(S)
    void rebuild() {
        for (int i = S - 1; i > 0; i--)
            value[i] = value[2 * i] + value[2 * i + 1];
    }

    // Replaces the value at index i with v in O(logS)
    void upd(int i, T v) {
        i += S;
        value[i] = v;
        while (i > 1) {
            i /= 2;
            value[i] = value[2 * i] + value[2 * i + 1];
        }
    }

    // Returns the sum of the values at indices [i, j] in O(logS)
    T query(int i, int j) {
        T res_left, res_right;
        for (i += S, j += S; i <= j; i /= 2, j /= 2) {
            if ((i&1) == 1) res_left = res_left + value[i++];
            if ((j&1) == 0) res_right = value[j--] + res_right;
        }
        return res_left + res_right;
    }
};

template<typename T> struct full_seg_tree : seg_tree<T> {
    int _S;
    full_seg_tree<T>(int _S) : _S(_S), seg_tree<T>(1 << (32 - __builtin_clz(_S - 1))) { }

    /*
     * Returns in O(logS) the first index i such that pred(query(0, i)) evaluates to true.
     * Returns S if no such i exists.
     * Requires that pred(query(0, i)) is non-decreasing in i.
     */
    int lower_bound(function<bool(T)> pred) {
        if (!pred(this->value[1])) return this->_S;
        T pref;
        int v = 1;
        for (int i = 0, j = this->S - 1; v < this->S; ) {
            int m = (j + i) / 2;
            if (pred(pref + this->value[2 * v])) {
                j = m;
                v = 2 * v;
            } else {
                pref = pref + this->value[2 * v];
                i = m + 1;
                v = 2 * v + 1;
            }
        }
        return v - this->S;
    }
};


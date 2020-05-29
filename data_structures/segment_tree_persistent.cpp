template<typename T, typename F, typename I = int>
struct segment_tree_persistent {
    int SZ;
    T id; F tt;
    vector<vector<pair<I, T>>> table;

    segment_tree_persistent(int SZ_, T id_, F tt_) : SZ(SZ_), id(id_), tt(tt_) {
        table.resize(2 * SZ);
    }

    T __get(int i) const { return table[i].empty() ? id : table[i].back().s; }

    // Reads table entry i as it was before moment w (excluding updates with w' >= w)
    T __get(int i, I w) const {
        static auto cmp = [](const pair<I, T>& a, const pair<I, T>& b) { return a.f < b.f; };
        auto it = lb(all(table[i]), mp(w, id), cmp);
        return it != table[i].begin() ? prev(it)->s : id;
    }

    I w_last = numeric_limits<I>::min();
    // Replaces the element at index i with v during moment w
    void replace(int i, T v, I w) {
        assert(w >= w_last), w_last = w;
        table[i += SZ].eb(w, v);
        for (i /= 2; i; i /= 2) {
            table[i].eb(w, tt(__get(2 * i), __get(2 * i + 1)));
        }
    }

    // Accumulates the elements at indices in [i, j) as they were before moment w
    T operator()(int i, int j, I w) const {
        T left{}, right{};
        for (i += SZ, j += SZ; i < j; i /= 2, j /= 2) {
            if (i&1) left = tt(left, __get(i++, w));
            if (j&1) right = tt(__get(--j, w), right);
        }
        return tt(left, right);
    }
};

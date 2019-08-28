template<typename T, typename I> struct persistent_segment_tree {
    using F = function<T(const T&, const T&)>;

    size_t SZ;
    F combine;
    vector<vector<pair<I, T>>> table;

    T get(int i) const { return sz(table[i]) ? table[i].back().s : T{}; }
    T get(int i, I w) const {
        static auto cmp = [](const pair<I, T>& a, const pair<I, T>& b) { return a.f < b.f; };
        auto it = ub(all(table[i]), mp(w, T{}), cmp);
        return it != table[i].begin() ? prev(it)->s : T{};
    }

    persistent_segment_tree<T, I>(size_t _SZ, F combine) : SZ(_SZ), combine(combine) {
        table.resize(2 * SZ);
        FORd (i, SZ, sz(table)) table[i].reserve(1);
        FORd (i, 1, SZ) table[i].reserve(table[2*i].capacity() + table[2*i+1].capacity());
    }

    void replace(int i, T v, I w) {
        table[i += SZ].eb(w, v);
        for (i /= 2; i; i /= 2)
            table[i].eb(w, combine(get(2 * i), get(2 * i + 1)));
    }

    T operator[](int i) const { return get(SZ + i); }

    T operator()(int i, int j, I w) const {
        T left{}, right{};
        for (i += SZ, j += SZ; i < j; i /= 2, j /= 2) {
            if (i&1) left = combine(left, get(i++, w));
            if (j&1) right = combine(get(--j, w), right);
        }
        return combine(left, right);
    }
};

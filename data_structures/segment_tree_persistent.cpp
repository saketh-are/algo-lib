template<typename T, typename F, typename I = int>
struct segment_tree_persistent {
    struct node {
        T val;
        int left, right;
    };

    int SZ; T tid; F tt;
    vector<node> data;
    vector<pair<I, int>> root;

    segment_tree_persistent() {}
    segment_tree_persistent(int SZ_, T tid_, F tt_) : tid(tid_), tt(tt_) {
        SZ = 1 << (32 - __builtin_clz(SZ_ - 1));
        data.resize(2 * SZ);
        for (int i = 0; i < SZ; i++)
            data[SZ + i] = { tid, -1, -1};
        for (int i = SZ - 1; i; i--)
            data[i] = { tt(data[2 * i].val, data[2 * i + 1].val), 2 * i, 2 * i + 1 };
    }

    // Assigns value v to the element at index i during moment w
    void assign(int i, T v, I w, bool replace = true) {
        assert(0 <= i && i < SZ && (root.empty() || root.back().first <= w));
        root.emplace_back(w, __assign(i, v, w, root.empty() ? 1 : root.back().second, 0, SZ, replace));
    }
    int __assign(int i, T v, I w, int loc, int L, int R, bool replace) {
        if (R - L == 1) {
            data.push_back({ replace ? v : tt(data[loc].val, v), -1, -1 });
        } else {
            int M = L + (R - L) / 2;
            int left  = i <  M ? __assign(i, v, w, data[loc].left,  L, M, replace) : data[loc].left;
            int right = M <= i ? __assign(i, v, w, data[loc].right, M, R, replace) : data[loc].right;
            data.push_back({ tt(data[left].val, data[right].val), left, right });
        }
        return data.size() - 1;
    }

    // Accumulates the elements at indices in [i, j) as they were before moment w
    T accumulate(int i, int j, I w) const {
        if (i >= j) return tid;
        assert(0 <= i && j <= SZ);
        return __accumulate(i, j, w, tid, root_before(w), 0, SZ);
    }
    int root_before(I w) const {
        static auto cmp = [](pair<I, int> a, pair<I, int> b) { return a.first < b.first; };
        auto it = lower_bound(root.begin(), root.end(), make_pair(w, tid), cmp);
        return it != root.begin() ? prev(it)->second : 1;
    }
    T __accumulate(int i, int j, I w, T init, int loc, int L, int R) const {
        if (L == i && j == R) {
            init = tt(init, data[loc].val);
        } else {
            int M = L + (R - L) / 2;
            if (i < M) init = __accumulate(i, min(j, M), w, init, data[loc].left,  L, M);
            if (M < j) init = __accumulate(max(i, M), j, w, init, data[loc].right, M, R);
        }
        return init;
    }
};

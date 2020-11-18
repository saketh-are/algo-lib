template<typename T, typename F, typename I = int>
struct segment_tree_persistent {
    struct node {
        T val;
        int left, right;
    };
    struct snapshot {
        I when;
        int root;
        int data_size;
        bool operator < (const snapshot &o) const { return when < o.when; }
    };

    int SZ; T tid; F tt;
    vector<node> data;
    vector<snapshot> history;

    segment_tree_persistent() {}
    segment_tree_persistent(int SZ_, T tid_, F tt_) : tid(tid_), tt(tt_) {
        SZ = 1 << (32 - __builtin_clz(SZ_ - 1));

        data.push_back({ tid, -1, -1 });
        for (int loc = 1; loc <= __builtin_ctz(SZ); loc++)
            data.push_back({ tt(data[loc - 1].val, data[loc - 1].val), loc - 1, loc - 1 });

        history.push_back({ numeric_limits<I>::min(), int(data.size()) - 1, int(data.size()) });
    }

    void replace           (int i, T val, I when) { __modify_leaf(i, val, when, true ); }
    void combine_and_assign(int i, T val, I when) { __modify_leaf(i, val, when, false); }

    void __modify_leaf(int i, T v, I w, bool replace) {
        assert(0 <= i && i < SZ && history.back().when <= w);

        int current_root = history.back().root;
        if (history.back().when == w) history.pop_back();

        int immutable = history.back().data_size;
        int updated_root = __modify_leaf(i, v, current_root, 0, SZ, immutable, replace);
        history.push_back({ w, updated_root, int(data.size()) });
    }
    int __modify_leaf(int i, T v, int loc, int L, int R, int immutable, bool replace) {
        node updated;
        if (R - L == 1) {
            updated = { replace ? v : tt(data[loc].val, v), -1, -1 };
        } else {
            int M = L + (R - L) / 2;
            int left  = i <  M ? __modify_leaf(i, v, data[loc].left,  L, M, immutable, replace) : data[loc].left;
            int right = M <= i ? __modify_leaf(i, v, data[loc].right, M, R, immutable, replace) : data[loc].right;
            updated = { tt(data[left].val, data[right].val), left, right };
        }
        if (loc < immutable) {
            loc = int(data.size());
            data.push_back(updated);
        } else {
            data[loc] = updated;
        }
        return loc;
    }

    // Accumulates the elements at indices in [i, j) as they were before moment w
    T accumulate(int i, int j, I w) const {
        if (i >= j) return tid;
        assert(0 <= i && j <= SZ);
        int root_before_w = prev(lower_bound(history.begin(), history.end(), snapshot{ w, -1, -1 }))->root;
        return __accumulate(i, j, tid, root_before_w, 0, SZ);
    }
    T __accumulate(int i, int j, T init, int loc, int L, int R) const {
        if (L == i && j == R) {
            init = tt(init, data[loc].val);
        } else {
            int M = L + (R - L) / 2;
            if (i < M) init = __accumulate(i, min(j, M), init, data[loc].left,  L, M);
            if (M < j) init = __accumulate(max(i, M), j, init, data[loc].right, M, R);
        }
        return init;
    }
};

template<typename T, typename U, typename TT, typename UU, typename UT, typename I = int>
struct segment_tree_persistent_lazy {
    struct node {
        T val; U upd;
        int left, right;
    };

    int SZ; T tid; U uid; TT tt; UU uu; UT ut;
    vector<node> data;
    vector<pair<I, int>> root;

    segment_tree_persistent_lazy() {}
    segment_tree_persistent_lazy(int SZ_, T tid_, U uid_, TT tt_, UU uu_, UT ut_) :
            tid(tid_), uid(uid_), tt(tt_), uu(uu_), ut(ut_) {
        SZ = 1 << (32 - __builtin_clz(SZ_ - 1));

        data.push_back({ tid, uid, -1, -1 });
        for (int sz = 2; sz <= SZ; sz *= 2)
            data.push_back({ tt(data.back().val, data.back().val), uid, int(data.size()), int(data.size()) });
    }

    int __apply(int loc, U u) {
        const node &cur = data[loc];
        data.push_back({ ut(u, cur.val), uu(u, cur.upd), cur.left, cur.right });
        return int(data.size()) - 1;
    }

    void __propagate(int loc) {
        if (memcmp(&data[loc].upd, &uid, sizeof(U))) {
            data[loc].left  = __apply(data[loc].left,  data[loc].upd);
            data[loc].right = __apply(data[loc].right, data[loc].upd);
            data[loc].upd = uid;
        }
    }

    // Assigns value v to the element at index i during moment w
    void assign(int i, T v, I w, bool replace = true) {
        assert(0 <= i && i < SZ && (root.empty() || root.back().first <= w));
        root.emplace_back(w, __assign(i, v, w, root.empty() ? __builtin_ctz(SZ) : root.back().second, 0, SZ, replace));
    }
    int __assign(int i, T v, I w, int loc, int L, int R, bool replace) {
        if (R - L == 1) {
            data.push_back({ replace ? v : tt(data[loc].val, v), uid, -1, -1 });
        } else {
            __propagate(loc);
            int M = L + (R - L) / 2;
            int left  = i <  M ? __assign(i, v, w, data[loc].left,  L, M, replace) : data[loc].left;
            int right = M <= i ? __assign(i, v, w, data[loc].right, M, R, replace) : data[loc].right;
            data.push_back({ tt(data[left].val, data[right].val), uid, left, right });
        }
        return int(data.size()) - 1;
    }

    // Applies update u to the elements at indices in [i, j) during moment w
    void update(int i, int j, U u, I w) {
        if (i >= j) return;
        assert(0 <= i && j <= SZ && (root.empty() || root.back().first <= w));
        root.emplace_back(w, __update(i, j, u, w, root.empty() ? __builtin_ctz(SZ) : root.back().second, 0, SZ));
    }
    int __update(int i, int j, U u, I w, int loc, int L, int R) {
        if (L == i && j == R) {
            return __apply(loc, u);
        } else {
            __propagate(loc);
            int M = L + (R - L) / 2;
            int left  = i < M ? __update(i, min(j, M), u, w, data[loc].left,  L, M) : data[loc].left;
            int right = M < j ? __update(max(i, M), j, u, w, data[loc].right, M, R) : data[loc].right;
            data.push_back({ tt(data[left].val, data[right].val), uid, left, right });
            return int(data.size()) - 1;
        }
    }

    // Accumulates the elements at indices in [i, j) as they were before moment w
    T accumulate(int i, int j, I w) {
        if (i >= j) return tid;
        assert(0 <= i && j <= SZ);
        return __accumulate(i, j, w, tid, root_before(w), 0, SZ);
    }
    int root_before(I w) const {
        static auto cmp = [](pair<I, int> a, pair<I, int> b) { return a.first < b.first; };
        auto it = lower_bound(root.begin(), root.end(), make_pair(w, tid), cmp);
        return it != root.begin() ? prev(it)->second : __builtin_ctz(SZ);
    }
    T __accumulate(int i, int j, I w, T init, int loc, int L, int R) {
        if (L == i && j == R) {
            init = tt(init, data[loc].val);
        } else {
            __propagate(loc);
            int M = L + (R - L) / 2;
            if (i < M) init = __accumulate(i, min(j, M), w, init, data[loc].left,  L, M);
            if (M < j) init = __accumulate(max(i, M), j, w, init, data[loc].right, M, R);
        }
        return init;
    }
};

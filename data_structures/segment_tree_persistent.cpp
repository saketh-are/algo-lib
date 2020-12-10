#include <vector>
#include <limits>
#include <cassert>
#include <algorithm>

template<typename T, typename BinaryOperation, typename Timestamp = int>
struct segment_tree_persistent {
    struct node {
        T v;
        int left, right;
    };
    struct snapshot {
        Timestamp t;
        int root;
        int data_size;
        bool operator < (const snapshot &o) const { return t < o.t; }
    };

    int SZ;
    T identity;
    BinaryOperation TT;
    std::vector<node> data;
    std::vector<snapshot> history;

    segment_tree_persistent() {}

    segment_tree_persistent(int _SZ, T _identity, BinaryOperation _TT) : identity(_identity), TT(_TT) {
        SZ = 1 << (32 - __builtin_clz(_SZ - 1));

        data.push_back({ identity, -1, -1 });
        for (int loc = 1; loc <= __builtin_ctz(SZ); loc++)
            data.push_back({ tt(data[loc - 1].val, data[loc - 1].val), loc - 1, loc - 1 });

        history.push_back({ std::numeric_limits<Timestamp>::min(), int(data.size()) - 1, int(data.size()) });
    }

    // Assigns v at index i during moment t
    void assign(int i, T v, Timestamp t) {
        modify_leaf(i, v, t, true);
    }

    // Replaces the current value at index i with TT(current value, v) during moment t
    void combine_and_assign(int i, T v, Timestamp t) {
        modify_leaf(i, v, t, false);
    }

    // Accumulates the elements at indices in [i, j) as they were before t (after all assignments with t' < t)
    T accumulate(int i, int j, Timestamp t) const {
        if (i >= j) return identity;
        assert(0 <= i && j <= SZ);
        int root_before_t = std::prev(std::lower_bound(history.begin(), history.end(), snapshot{ t, -1, -1 }))->root;
        return accumulate(i, j, identity, root_before_t, 0, SZ);
    }

private:
    void modify_leaf(int i, T v, Timestamp t, bool overwrite) {
        assert(0 <= i && i < SZ && history.back().t <= t);

        int current_root = history.back().root;
        if (history.back().t == t) history.pop_back();

        int immutable = history.back().data_size;
        int updated_root = modify_leaf(i, v, current_root, 0, SZ, immutable, overwrite);
        history.push_back({ t, updated_root, int(data.size()) });
    }

    int modify_leaf(int i, T v, int loc, int L, int R, int immutable, bool overwrite) {
        node updated;
        if (R - L == 1) {
            updated = { overwrite ? v : TT(data[loc].v, v), -1, -1 };
        } else {
            int M = L + (R - L) / 2;
            int left  = i <  M ? modify_leaf(i, v, data[loc].left,  L, M, immutable, overwrite) : data[loc].left;
            int right = M <= i ? modify_leaf(i, v, data[loc].right, M, R, immutable, overwrite) : data[loc].right;
            updated = { tt(data[left].v, data[right].v), left, right };
        }
        if (loc < immutable) {
            loc = int(data.size());
            data.push_back(updated);
        } else {
            data[loc] = updated;
        }
        return loc;
    }

    T accumulate(int i, int j, T init, int loc, int L, int R) const {
        if (L == i && j == R) {
            init = TT(init, data[loc].val);
        } else {
            int M = L + (R - L) / 2;
            if (i < M) init = accumulate(i, std::min(j, M), init, data[loc].left,  L, M);
            if (M < j) init = accumulate(std::max(i, M), j, init, data[loc].right, M, R);
        }
        return init;
    }
};

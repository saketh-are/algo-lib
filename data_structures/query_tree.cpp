#include <vector>
#include <map>

/*
 * Accepts a sequence of operations inserting and deleting
 * elements of type T in a data structure of type D, and
 * a function "eval" on type D computing a result of type V.
 *
 * Computes and returns the results obtained when applying
 * the given operations sequentially.
 *
 * Requires D to expose a function to undo the most recently
 * performed insertions. Does not require D to support deletion
 * of arbitrary elements. Breaks amortized complexity.
 */
template<typename D, typename T, typename V>
std::vector<V> query_tree(D& ds, const std::vector<std::pair<bool, T>>& ops,
        const auto& apply, const auto& eval) {
    std::vector<int> match(ops.size(), ops.size());
    std::map<T, int> inserted;
    for (int op = 0; op < int(ops.size()); op++) {
        const auto &[ins, t] = ops[op];
        if (ins) inserted[t] = op;
        else {
            auto it = inserted.find(t);
            match[op] = it->second;
            match[it->second] = op;
            inserted.erase(it);
        }
    }

    std::vector<V> results(ops.size() + 1);
    results[0] = eval(ds);

    auto dfs = [&](auto& self, int l, int r) -> void {
        if (r - l == 1) {
            if (match[l] > l) apply(ds, ops[l].second);
            results[r] = eval(ds);
            if (match[l] > l) ds.rewind(ds.opcount() - 1);
            return;
        }

        const int mid = (l + r) / 2;
        const int before = ds.opcount();

        if (l < mid) {
            for (int i = mid; i < r; i++)
                if (match[i] < l) apply(ds, ops[i].second);
            self(self, l, mid);
            ds.rewind(before);
        }

        if (mid < r) {
            for (int i = l; i < mid; i++)
                if (match[i] >= r) apply(ds, ops[i].second);
            self(self, mid, r);
            ds.rewind(before);
        }
    };
    dfs(dfs, 0, int(ops.size()));

    return results;
}

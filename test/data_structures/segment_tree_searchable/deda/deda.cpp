// Problem: https://oj.uz/problem/submit/COCI17_deda

#include <iostream>
#include <climits>
using namespace std;

// {{{ data_structures/segment_tree_searchable }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    auto smaller = [](int a, int b) { return min(a, b); };
    searchable_segment_tree<int, decltype(smaller)> sst(N, INT_MAX, smaller);

    for (int q = 0; q < Q; q++) {
        char op;
        int x, y;
        cin >> op >> x >> y;

        if (op == 'M') {
            sst.assign(y - 1, x);
        } else {
            int ans = sst.binary_search(y - 1, [&x](int v) { return v <= x; });
            cout << (ans > N ? -1 : ans) << "\n";
        }
    }

    return 0;
}

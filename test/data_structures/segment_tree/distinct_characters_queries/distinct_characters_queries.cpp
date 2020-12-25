// Problem: https://codeforces.com/problemset/problem/1234/D

#include <iostream>
using namespace std;

// {{{ data_structures/segment_tree }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    segment_tree<int, bit_or<int>> st(int(s.size()), 0, bit_or<int>());

    st.assign([&s](int i) { return 1 << (s[i] - 'a'); });

    int Q;
    cin >> Q;
    for (int q = 0; q < Q; q++) {
        int t;
        cin >> t;
        if (t == 1) {
            int pos;
            char c;
            cin >> pos >> c;

            st.assign(pos - 1, 1 << (c - 'a'));
        } else {
            int l, r;
            cin >> l >> r;

            cout << __builtin_popcount(st.accumulate(l - 1, r)) << "\n";
        }
    }

    return 0;
}

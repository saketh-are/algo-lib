// Problem: https://codeforces.com/contest/914/problem/F

#include <iostream>
using namespace std;

// {{{ strings/mutable_string.cpp }}}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string init;
    cin >> init;

    mutable_string<'a', 26> s(init.begin(), init.end());

    int Q;
    cin >> Q;

    for (int q = 0; q < Q; q++) {
        int t;
        cin >> t;

        if (t == 1) {
            int i;
            char c;
            cin >> i >> c;

            s.assign(i - 1, c);
        } else {
            int l, r;
            string y;
            cin >> l >> r >> y;

            cout << s.count_matches_in_substring(y.begin(), y.end(), l - 1, r) << "\n";
        }
    }
}

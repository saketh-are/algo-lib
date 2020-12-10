// Problem: https://codeforces.com/contest/963/problem/D

#include <iostream>
#include <vector>
using namespace std;

// {{{ strings/trie.cpp }}}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int Q;
    cin >> Q;

    vector<int> k(Q);
    vector<string> m(Q);

    for (int q = 0; q < Q; q++) {
        cin >> k[q] >> m[q];
    }

    trie<'a', 26> tr(m.begin(), m.end());

    vector<vector<int>> occur = tr.indices_of_matches(s.begin(), s.end());

    for (int q = 0; q < Q; q++) {
        if (int(occur[q].size()) < k[q]) {
            cout << -1 << "\n";
            continue;
        }

        int ans = int(s.size());

        for (int f = 0; f <= int(occur[q].size()) - k[q]; f++) {
            int begin = occur[q][f];
            int end = occur[q][f + k[q] - 1] + int(m[q].size());

            ans = min(ans, end - begin);
        }

        cout << ans << "\n";
    }
}

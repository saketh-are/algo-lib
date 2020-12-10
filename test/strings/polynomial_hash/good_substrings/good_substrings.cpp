// Problem: https://codeforces.com/contest/271/problem/D

#include <iostream>
#include <algorithm>
using namespace std;

// {{{ strings/polynomial_hash.cpp }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    array<bool, 26> good;
    for (int i = 0; i < 26; i++) {
        char c;
        cin >> c;
        good[i] = c == '1';
    }

    int K;
    cin >> K;

    using H = polynomial_hash<modnum<int(1e9 + 7)>, 5, 256>;

    vector<H> s_prefixes = H::get_prefixes(s.begin(), s.end());

    vector<H> good_substrings;

    for (int head = 0; head < int(s.size()); head++) {
        int bad = 0;
        for (int tail = head; tail < int(s.size()); tail++) {
            if (!good[s[tail] - 'a'])
                bad++;

            if (bad > K)
                break;

            good_substrings.push_back(H::get_substring(s_prefixes, head, tail + 1));
        }
    }

    sort(good_substrings.begin(), good_substrings.end());

    cout << unique(good_substrings.begin(), good_substrings.end()) - good_substrings.begin() << endl;
}

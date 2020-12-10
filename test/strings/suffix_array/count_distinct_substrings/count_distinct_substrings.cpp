// Problem: https://judge.yosupo.jp/problem/number_of_substrings

#include <iostream>
#include <numeric>
using namespace std;

// {{{ strings/suffix_array.cpp }}}

int main() {
    string s;
    cin >> s;

    suffix_array<NEG_INF> sa(s.begin(), s.end());

    int64_t ans = int64_t(s.size()) * (s.size() + 1) / 2
        - accumulate(sa.lcp_between_ranks.begin(), sa.lcp_between_ranks.end(), int64_t(0));

    printf("%jd\n", ans);
}

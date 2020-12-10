// Problem: https://judge.yosupo.jp/problem/suffixarray

#include <string>
#include <iostream>
using namespace std;

// {{{ strings/suffix_array.cpp }}}

int main() {
    string S;
    cin >> S;

    suffix_array<NEG_INF> sa(S.begin(), S.end());

    for (int i = 0; i < int(S.size()); i++) {
        if (i) cout << " ";
        cout << sa.suffix_at_rank(i);
    }
    cout << endl;
}

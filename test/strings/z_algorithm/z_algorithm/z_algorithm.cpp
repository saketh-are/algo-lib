// Problem: https://judge.yosupo.jp/problem/zalgorithm

// {{{ strings/z_algorithm.cpp }}}

#include <string>
#include <iostream>
using namespace std;

int main() {
    string S;
    cin >> S;

    auto z = z_algorithm(S.begin(), S.end());

    for (int i = 0; i < int(z.size()); i++) {
        if (i) cout << " ";
        cout << z[i];
    }
    cout << endl;
}

// Problem: https://judge.yosupo.jp/problem/enumerate_palindromes

#include <string>
#include <iostream>
using namespace std;

// {{{ strings/palindromes.cpp }}}

int main() {
    string S;
    cin >> S;

    palindromes p(S.begin(), S.end());

    for (int i = 0; i < int(S.size()); i++) {
        if (i) cout << " ";
        cout << p.longest_centered_at(i);

        if (i+1 < int(S.size()))
            cout << " " << p.longest_centered_right_of(i);
    }
    cout << endl;
}

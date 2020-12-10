// Problem: https://www.spoj.com/problems/NAJPF/

#include <iostream>
#include <vector>
using namespace std;

// {{{ strings/knuth_morris_pratt.cpp }}}

int main() {
    int T;
    cin >> T;

    for (int t = 0; t < T; t++) {
        if (t) cout << "\n";

        string text, pattern;
        cin >> text >> pattern;

        knuth_morris_pratt kmp(pattern.begin(), pattern.end());

        std::vector<int> matches = kmp.indices_of_matches(text.begin(), text.end());

        if (matches.empty()) {
            cout << "Not Found\n";
        } else {
            cout << matches.size() << "\n";
            for (int i = 0; i < int(matches.size()); i++) {
                if (i) cout << " ";
                cout << matches[i] + 1;
            }
            cout << "\n";
        }
    }
}


// Problem: https://judge.yosupo.jp/problem/number_of_substrings

#include <iostream>
#include <numeric>
using namespace std;

// {{{ strings/suffix_automaton.cpp }}}

int main() {
    string s;
    cin >> s;

    suffix_automaton<'a', 26> sa(s.begin(), s.end());

    vector<int64_t> reachable_endpos(int(sa.data.size()));

    for (int loc : sa.reverse_length_order) {
        reachable_endpos[loc] = sa.ct_end_pos[loc] > 0;
        for (int nbr : sa.data[loc].transitions)
            if (nbr != -1)
                reachable_endpos[loc] += reachable_endpos[nbr];
    }

    printf("%jd\n", reachable_endpos[0]);
}

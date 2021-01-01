// Problem: https://codeforces.com/problemset/problem/710/F

#include <iostream>
#include <string>
using namespace std;

// {{{ strings/trie }}}

// {{{ data_structures/static_to_dynamic_transformation }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    static_to_dynamic_transformation<string, trie<'a', 26>>
        inserted, deleted;

    int Q;
    cin >> Q;

    for (int q = 0; q < Q; q++) {
        int type;
        string str;
        cin >> type >> str;

        if (type == 1)
            inserted.insert(str);
        else if (type == 2)
            deleted.insert(str);
        else {
            auto query = [&](const trie<'a', 26> &tr) {
                return int(tr.count_total_matches(str.begin(), str.end()));
            };

            int inserted_matches = inserted.accumulate(query, 0, plus<int>());
            int deleted_matches = deleted.accumulate(query, 0, plus<int>());

            cout << inserted_matches - deleted_matches << endl;
        }
    }

    return 0;
}

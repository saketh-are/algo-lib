// Problem: https://codeforces.com/problemset/problem/1290/C

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

// {{{ data_structures/union_find_bipartite }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    string s;
    cin >> s;

    vector<vector<int>> switches(N);

    for (int sw = 0; sw < K; sw++) {
        int C;
        cin >> C;
        for (int i = 0; i < C; i++) {
            int e;
            cin >> e;
            switches[e - 1].push_back(sw);
        }
    }

    union_find_bipartite uf(K);

    for (int lamp = 0; lamp < N; lamp++) {
        switch (switches[lamp].size()) {
            case 0:
                assert(s[lamp] == '1');
                break;
            case 1:
                assert(uf.can_constrain_node_to_side(switches[lamp][0], s[lamp] != '1'));
                assert(uf.constrain_node_to_side(switches[lamp][0], s[lamp] != '1'));
                break;
            case 2:
                assert(uf.can_add_constraint_on_nodes(switches[lamp][0], switches[lamp][1], s[lamp] != '1'));
                assert(uf.unite(switches[lamp][0], switches[lamp][1], s[lamp] != '1').component_is_bipartite);
                break;
            default:
                assert(false);
        }

        cout << uf.min_nodes_on_side_1 << "\n";
    }

    return 0;
}

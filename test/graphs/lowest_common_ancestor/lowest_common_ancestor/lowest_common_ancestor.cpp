// Problem: https://judge.yosupo.jp/problem/lca

#include <iostream>
using namespace std;

// {{{ graphs/lowest_common_ancestor.cpp }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    tree<edge> t(N, 0);
    re(t, PARENT_LIST, 0);

    lowest_common_ancestor<edge> lca(t);

    for (int q = 0; q < Q; q++) {
        int u, v;
        cin >> u >> v;

        cout << lca.lca(u, v) << "\n";
    }

    return 0;
}


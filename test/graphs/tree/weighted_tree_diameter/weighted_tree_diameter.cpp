// {{{ graphs/tree.cpp }}}

#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    using E = edge_with_data<int>;

    tree<E> t(N, 0);

    re(t, EDGE_LIST, 0, [](E &e) { cin >> e.data; });

    vector<int64_t> dist0(N);
    for (int u : t.preorder)
        t.for_each_child(u, [&](E e) { dist0[e.get_nbr(u)] = dist0[u] + e.data; });

    int x = int(max_element(dist0.begin(), dist0.end()) - dist0.begin());

    t.reroot(x);

    vector<int64_t> distx(N);
    for (int u : t.preorder)
        t.for_each_child(u, [&](E e) { distx[e.get_nbr(u)] = distx[u] + e.data; });

    int y = int(max_element(distx.begin(), distx.end()) - distx.begin());

    cout << distx[y];

    vector<int> path = {y};
    while (y != x) {
        y = t.parent[y];
        path.push_back(y);
    }

    cout << " " << path.size() << "\n";

    for (int i = 0; i < int(path.size()); i++) {
        if (i) cout << " ";
        cout << path[i];
    }
    cout << "\n";
}

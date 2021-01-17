// Problem: https://codeforces.com/problemset/problem/932/F

#include <iostream>
using namespace std;

// {{{ data_structures/line_container }}}

// {{{ graphs/tree }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    vector<int> a(N);
    for (int i = 0; i < N; i++)
        cin >> a[i];

    vector<int> b(N);
    for (int i = 0; i < N; i++)
        cin >> b[i];

    tree<edge> tr(N, 0);
    re(tr, EDGE_LIST, 1);

    vector<int64_t> min_cost_path(N, 1e10);
    vector<line_container<int64_t>> leaf_paths(N);

    for (int u : tr.reverse_preorder) {
        bool has_child = false;

        tr.for_each_child(u, [&](edge e) {
            has_child = true;

            int child = e.get_nbr(u);

            if (leaf_paths[child].size() > leaf_paths[u].size())
                swap(leaf_paths[u], leaf_paths[child]);

            for (const line<int64_t> &l : leaf_paths[child])
                leaf_paths[u].insert_line(l.a, l.b);

            leaf_paths[child].clear();
        });

        if (!has_child)
            min_cost_path[u] = 0;
        else
            min_cost_path[u] = -leaf_paths[u].get_maximum(a[u]);

        leaf_paths[u].insert_line(-b[u], -min_cost_path[u]);
    }

    for (int u = 0; u < N; u++) {
        if (u) cout << " ";
        cout << min_cost_path[u];
    }
    cout << endl;

    return 0;
}

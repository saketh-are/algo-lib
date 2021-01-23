// Problem: https://codeforces.com/problemset/problem/587/F

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// {{{ strings/trie }}}

// {{{ graphs/tree }}}

// {{{ data_structures/sqrt_decomposition_point_query }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, Q;
    cin >> N >> Q;

    vector<string> names(N);
    for (int i = 0; i < N; i++)
        cin >> names[i];

    vector<int> L(Q), R(Q), K(Q);

    vector<vector<int>> queries(N);

    vector<vector<int>> subtract(N), add(N);

    for (int q = 0; q < Q; q++) {
        cin >> L[q] >> R[q] >> K[q];
        --L[q], --R[q], --K[q];

        if (names[K[q]].size() > 300u) {
            queries[K[q]].push_back(q);
        } else {
            subtract[L[q]].push_back(q);
            add[R[q]].push_back(q);
        }
    }

    vector<int64_t> ans(Q);

    trie<'a', 26> tr(names.begin(), names.end());

    vector<int64_t> prefixes(N + 1);

    for (int i = 0; i < N; i++) {
        if (queries[i].empty())
            continue;

        vector<int> matches = tr.count_matches(names[i].begin(), names[i].end());

        for (int j = 0; j < N; j++)
            prefixes[j + 1] = prefixes[j] + matches[j];

        for (int q : queries[i])
            ans[q] = prefixes[R[q] + 1] - prefixes[L[q]];
    }

    const int V = int(tr.data.size());

    tree<edge> suffix_link_tree(V, 0);

    for (int i = 1; i < V; i++)
        suffix_link_tree.add_edge(i, tr.data[i].suffix_link);

    suffix_link_tree.init();

    vector<int> preorder_index(V);

    for (int i = 0; i < V; i++)
        preorder_index[suffix_link_tree.preorder[i]] = i;

    sqrt_decomposition_point_query st(V, 0, plus<int>());

    auto sum_trie_path = [&](int w) {
        int64_t res = 0;

        int loc = 0;
        for (char c : names[w]) {
            loc = tr.child_link(loc, c);
            res += st.read(preorder_index[loc]);
        }

        return res;
    };

    for (int i = 0; i < N; i++) {
        for (int q : subtract[i])
            ans[q] -= sum_trie_path(K[q]);

        int v = tr.dictionary_word_links[i];
        st.add_to_range(preorder_index[v], preorder_index[v] + suffix_link_tree.subtree_size[v], 1);

        for (int q : add[i])
            ans[q] += sum_trie_path(K[q]);
    }

    for (int64_t v : ans)
        cout << v << "\n";

    return 0;
}

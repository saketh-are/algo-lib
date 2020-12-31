// Problem: https://codeforces.com/gym/100551/problem/A

#ifdef ONLINE_JUDGE
    #include <fstream>
    std::ifstream cin("connect.in");
    std::ofstream cout("connect.out");
#else
    #include <iostream>
#endif

using namespace std;

// {{{ data_structures/union_find_rewindable }}}

int main() {
    int N, K;
    cin >> N >> K;

    vector<pair<int, int>> edge_sequence;

    vector<int> queries;

    for (int i = 0; i < K; i++) {
        char op;
        cin >> op;
        if (op == '?') {
            queries.push_back(int(edge_sequence.size()));
        } else {
            int u, v;
            cin >> u >> v;
            if (u > v) std::swap(u, v);
            edge_sequence.emplace_back(u - 1, v - 1);
        }
    }

    vector<int> component_counts = get_component_counts(N, edge_sequence);

    for (int q : queries)
        cout << component_counts[q] << "\n";

    return 0;
}

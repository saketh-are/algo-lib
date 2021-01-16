#include <iostream>
using namespace std;

// {{{ numeric/bignum_addpow2_compare }}}

// {{{ graphs/dijkstra }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M;
    cin >> N >> M;

    WeightedDirectedGraph<int> g(N);

    for (int e = 0; e < M; e++) {
        int u, v, w;
        cin >> u >> v >> w;
        --u, --v;

        g.add_directed_edge(u, v, w);
        g.add_directed_edge(v, u, w);
    }

    int S, T;
    cin >> S >> T;
    --S, --T;

    bignum_addpow2_compare nums;

    ShortestPathTree sp(g, {S}, 0,
        [&](int path, int edge) { return nums.add_pow2(path, edge); },
        [&](int a, int b) { return nums.less_than(a, b); }
    );

    if (!sp.is_reachable(T)) {
        cout << -1 << endl;
        return 0;
    }

    int dist = sp.distance(T);
    vector<int> path = sp.list_vertices_on_path(T);

    cout << nums.nums[dist].hash.data[0] << endl;

    cout << path.size() << endl;

    for (int v : path)
        cout << v + 1 << " ";
    cout << endl;

    return 0;
}

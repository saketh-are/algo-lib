// Problem: https://atcoder.jp/contests/abc192/tasks/abc192_e

#include <iostream>
using namespace std;

// {{{ graphs/dijkstra }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, X, Y;
    cin >> N >> M >> X >> Y;
    --X, --Y;

    struct EdgeWeight {
        int t, k;
    };

    WeightedDirectedGraph<EdgeWeight> g(N);

    for (int i = 0; i < M; i++) {
        int a, b, t, k;
        cin >> a >> b >> t >> k;
        --a, --b;

        g.add_directed_edge(a, b, { t, k });
        g.add_directed_edge(b, a, { t, k });
    }

    using PathWeight = int64_t;

    auto JoinPathAndEdge = [](PathWeight path, EdgeWeight edge) {
        return (path + edge.k - 1) / edge.k * edge.k + edge.t;
    };

    auto PathWeightLessThan = less<int64_t>();

    ShortestPathTree<PathWeight> spt(g, {X}, 0, JoinPathAndEdge, PathWeightLessThan);

    cout << (spt.is_reachable(Y) ? spt.distance(Y) : -1) << endl;

    return 0;
}

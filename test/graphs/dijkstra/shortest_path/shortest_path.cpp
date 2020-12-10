#include <cstdio>
#include <functional>

// {{{ graphs/dijkstra.cpp }}}

int main() {
    int N, M, s, t;
    scanf("%d %d %d %d", &N, &M, &s, &t);

    WeightedDirectedGraph<int> graph(N);
    for (int i = 0; i < M; i++) {
        int a, b, c;
        scanf("%d %d %d", &a, &b, &c);

        graph.add_directed_edge(a, b, c);
    }

    ShortestPathTree<int64_t> tree(graph, {s}, 0, std::plus<int64_t>(), std::less<int64_t>());

    if (!tree.is_reachable(t)) {
        printf("-1\n");
        return 0;
    }

    std::vector<int> path = tree.list_vertices_on_path(t);

    printf("%jd %d\n", tree.distance(t), int(path.size()) - 1);

    for (int i = 0; i + 1 < int(path.size()); i++)
        printf("%d %d\n", path[i], path[i + 1]);
}


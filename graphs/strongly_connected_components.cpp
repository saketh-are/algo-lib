#include <vector>

struct scc {
    int components;
    std::vector<int> label;
};

// Labels strongly connected components in reverse topological order.
// That means for any edge u-->v label[u] >= label[v].
scc strongly_connected_components(const std::vector<std::vector<int>>& graph) {
    const int N = int(graph.size());

    int C = 0, V = 0, top = 0;
    std::vector<int> comp(N, -1), inx(N, -1), stack(N + 1, -1);

    auto tarjan = [&](auto&& self, int loc) -> int {
        stack[top++] = loc;
        int low = inx[loc] = V++;
        for (int nbr : graph[loc]) {
            if (inx[nbr] == -1) low = std::min(low, self(self, nbr));
            else if (comp[nbr] == -1) low = std::min(low, inx[nbr]);
        }
        if (low == inx[loc]) {
            while (stack[top] != loc)
                comp[stack[--top]] = C;
            C++;
        }
        return low;
    };

    for (int i = 0; i < N; i++) {
        if (inx[i] == -1)
            tarjan(tarjan, i);
    }
    return { C, comp };
}

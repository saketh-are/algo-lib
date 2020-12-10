// {{{ graphs/strongly_connected_components.cpp }}}

#include <vector>
#include <bitset>
#include <cassert>
#include <numeric>
#include <algorithm>

template<int MAXV>
std::vector<std::bitset<MAXV>> digraph_reachability(
        const std::vector<std::vector<int>> &graph) {
    int V = int(graph.size());
    assert(V <= MAXV);

    scc s = strongly_connected_components(graph);

    std::vector<std::bitset<MAXV>> component_reachability(s.components);

    for (int v = 0; v < s.components; v++)
        component_reachability[s.label[v]][v] = true;

    std::vector<int> order;
    std::iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
            [&](int u, int v) { return s.label[u] < s.label[v]; });

    for (int u : order)
        for (int v : graph[u])
            component_reachability[s.label[u]]
                |= component_reachability[s.label[v]];

    std::vector<std::bitset<MAXV>> reach(V);
    for (int v = 0; v < V; v++)
        reach[v] = component_reachability[s.label[v]];
    return reach;
}

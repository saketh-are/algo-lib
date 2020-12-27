// Problem: https://codeforces.com/contest/25/problem/D

#include <cstdio>
#include <vector>
#include <cassert>
using namespace std;

// {{{ data_structures/union_find }}}

int main() {
    int N;
    scanf("%d", &N);

    union_find uf(N);

    std::vector<pair<int, int>> redundant;

    for (int i = 0; i < N - 1; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        --u, --v;

        if (!uf.unite(u, v))
            redundant.emplace_back(u, v);
    }

    std::vector<int> component_roots;

    for (int i = 0; i < N; i++)
        if (uf.is_root(i))
            component_roots.push_back(i);

    assert(redundant.size() + 1 == component_roots.size());

    printf("%d\n", int(redundant.size()));

    for (int i = 0; i < int(redundant.size()); i++) {
        printf("%d %d %d %d\n",
                redundant[i].first + 1, redundant[i].second + 1,
                component_roots[i] + 1, component_roots[i + 1] + 1);
    }

    return 0;
}


// Problem: https://codeforces.com/contest/1767/problem/E

#include <cstdio>
#include <cstdint>
using namespace std;

// {{{ graphs/min_cost_vertex_cover.cpp }}}

int main() {
    int N, M;
    scanf("%d %d", &N, &M);

    vector<int> plat(N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &plat[i]);
        --plat[i];
    }

    vector<int> cost(M);
    for (int i = 0; i < M; i++) {
        scanf("%d", &cost[i]);
    }

    vector<int64_t> adj(M);

    for (int i = 0; i < N - 1; i++) {
        int a = plat[i];
        int b = plat[i + 1];

        adj[a] |= int64_t(1) << b;
        adj[b] |= int64_t(1) << a;
    }

    adj[plat[0]] |= int64_t(1) << plat[0];
    adj[plat[N - 1]] |= int64_t(1) << plat[N - 1];

    printf("%d\n", min_cost_vertex_cover<int, int>(M, cost, adj));

    return 0;
}

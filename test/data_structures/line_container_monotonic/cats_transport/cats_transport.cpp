// Problem: https://codeforces.com/contest/311/problem/B

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// {{{ data_structures/line_container_monotonic }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, M, P;
    cin >> N >> M >> P;

    vector<int> dist(N);

    for (int i = 0; i < N - 1; i++) {
        int d;
        cin >> d;

        dist[i + 1] = dist[i] + d;
    }

    vector<int> cats(M);

    for (int i = 0; i < M; i++) {
        int h, t;
        cin >> h >> t;
        cats[i] = t - dist[h - 1];
    }

    sort(cats.begin(), cats.end());

    vector<int64_t> prefixes(M + 1);

    for (int i = 0; i < M; i++)
        prefixes[i + 1] = prefixes[i] + cats[i];

    line_container_monotonic<int64_t> prev, next;

    prev.insert_line(0, 0);

    int64_t cost;

    for (int handler = 0; handler < P; handler++) {
        size_t position = 0;
        for (int take = 1; take <= M; take++) {
            cost = take * int64_t(cats[take - 1])
                - prefixes[take]
                - prev.get_maximum_monotonic(cats[take - 1], position);

            next.insert_line(take, -cost - prefixes[take]);
        }

        swap(prev, next);
        next.clear();
    }

    cout << cost << endl;

    return 0;
}

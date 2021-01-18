// Problem: https://codeforces.com/problemset/problem/755/F

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

// {{{ misc/subset_sum.cpp }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, K;
    cin >> N >> K;

    vector<int> giver(N);

    for (int i = 0; i < N; i++) {
        cin >> giver[i];
        --giver[i];
    }

    vector<int> cycle_lengths;

    vector<bool> visited(N);
    for (int i = 0; i < N; i++) {
        if (visited[i])
            continue;

        int length = 0;
        for (int t = i; !visited[t]; t = giver[t]) {
            visited[t] = true;
            length++;
        }

        cycle_lengths.push_back(length);
    }

    auto sums = subset_sum<1000000>(cycle_lengths.begin(), cycle_lengths.end());

    int minimum = K + !sums[K];

    int twos = 0, ones = 0;

    for (int length : cycle_lengths) {
        twos += length / 2;
        ones += length & 1;
    }

    int maximum = 2 * min(K, twos) + min(ones, K - min(K, twos));

    printf("%d %d\n", minimum, maximum);

    return 0;
}

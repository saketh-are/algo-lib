// Problem: https://codeforces.com/problemset/problem/755/F

#include <cstdio>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// {{{ misc/subset_sum.cpp }}}

int main() {
    int N, K;
    scanf("%d %d", &N, &K);

    std::vector<int> giver(N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &giver[i]);
        --giver[i];
    }

    std::vector<int> cycle_lengths;

    std::vector<bool> visited(N);
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

    std::vector<int> options;
    for (int length : cycle_lengths) {
        for (int i = 0; i < length / 2; i++)
            options.push_back(2);
        if (length&1)
            options.push_back(1);
    }
    std::sort(options.begin(), options.end(), greater<int>());

    int maximum = accumulate(options.begin(), options.begin() + min(K, int(options.size())), 0);

    printf("%d %d\n", minimum, maximum);

    return 0;
}

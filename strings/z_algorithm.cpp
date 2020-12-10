#include <vector>

/* Returns z[i] = the greatest length L such that
 * s[i, i+L) is identical to s[0, L).
 */
template<typename RandomAccessIterator>
std::vector<int> z_algorithm(RandomAccessIterator first, RandomAccessIterator last) {
    int N = int(last - first);

    std::vector<int> z(N);
    z[0] = N;

    for (int i = 1, l, r = -1; i < N; i++) {
        z[i] = r > i ? std::min(r - i, z[i - l]) : 0;
        while (i + z[i] < N && first[i + z[i]] == first[z[i]])
            z[i]++;
        if (i + z[i] > r)
            l = i, r = i + z[i];
    }

    return z;
};

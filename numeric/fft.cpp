#include <cassert>
#include <algorithm>
#include <type_traits>

const int FFT_CUTOFF = 150;

template<typename RandomAccessIterator>
void fft(RandomAccessIterator first, RandomAccessIterator last, bool invert) {
    int N = int(last - first);
    assert(__builtin_popcount(N) == 1);

    for (int i = 1, j = 0, k; i < N; i++) {
        for (k = N >> 1; (j ^= k) < k; k >>= 1);
        if (i < j) std::swap(first[i], first[j]);
    }

    using T = typename std::remove_reference<decltype(*first)>::type;

    for (int l = 1; l < N; l <<= 1) {
        for (int i = 0; i < N; i += 2 * l) {
            for (int j = 0; j < l; j++) {
                T w = T::unity_root(2 * l, invert ? -j : j);
                T u = first[i + j];
                T v = first[i + j + l] * w;
                first[i + j] = u + v;
                first[i + j + l] = u - v;
            }
        }
    }

    if (invert) {
        T N_inverse = T{N}.inv();

        for (int i = 0; i < N; i++)
            first[i] = first[i] * N_inverse;
    }
}

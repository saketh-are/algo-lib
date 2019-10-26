const int FFT_CUTOFF = 150;
template<typename T, bool INV> static void fft(vector<T>& a, int N) {
    assert(__builtin_popcount(N) == 1);
    for (int i = 1, j = 0, k; i < N; i++) {
        for (k = N >> 1; (j ^= k) < k; k >>= 1);
        if (i < j) swap(a[i], a[j]);
    }

    for (int l = 1; l < N; l <<= 1) {
        for (int i = 0; i < N; i += 2 * l) {
            for (int j = 0; j < l; j++) {
                T w = T::unity_root(2 * l, INV ? -j : j);
                T u = a[i + j];
                T v = a[i + j + l] * w;
                a[i + j] = u + v;
                a[i + j + l] = u - v;
            }
        }
    }
    if (INV) {
        T ninv = T{N}.inv();
        for (int i = 0; i < N; i++) a[i] = a[i] * ninv;
    }
}

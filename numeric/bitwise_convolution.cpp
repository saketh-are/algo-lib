namespace bitwise_convolution {
    template<typename T, bool INV> void transform(vector<T>& v, int N, array<int, 4> t) {
        assert(__builtin_popcount(N) == 1);
        for (int c : t) assert(abs(c) <= 1);
        int w = t[0], x = t[1], y = t[2], z = t[3];
        if (INV) swap(w, z), x *= -1, y *= -1;

        auto prod = [](int c, T v){ return c > 0 ? v : c < 0 ? -v : 0; };
        for (int l = 1; l < N; l <<= 1) {
            for (int i = 0; i < N; i += l << 1) {
                for (int j = 0; j < l; j++) {
                    T a = v[i + j];
                    T b = v[i + j + l];
                    v[i + j] = prod(w, a) + prod(x, b);
                    v[i + j + l] = prod(y, a) + prod(z, b);
                }
            }
        }
        if (INV) {
            const T dinv = 1 / T{w * z - x * y}.pow(__builtin_ctz(N));
            for (int i = 0; i < N; i++) v[i] *= dinv;
        }
    }

    constexpr array<int, 4>  OR(){ return {1,0,1,1}; }
    constexpr array<int, 4> AND(){ return {0,1,1,1}; }
    constexpr array<int, 4> XOR(){ return {1,1,1,-1}; }

    template<typename T>
    vector<T> conv(const vector<T>& a, const vector<T>& b, array<int, 4> t) {
        if (a.empty() || b.empty()) return {};
        int N = 1 << (32 - __builtin_clz(max(sz(a), sz(b)) - 1));
        static vector<T> fa, fb;
        if (N > sz(fa)) fa.resize(N), fb.resize(N);

        copy(all(a), fa.begin());
        fill(fa.begin() + sz(a), fa.begin() + N, 0);
        copy(all(b), fb.begin());
        fill(fb.begin() + sz(b), fb.begin() + N, 0);

        transform<T, false>(fa, N, t);
        transform<T, false>(fb, N, t);
        for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
        transform<T, true>(fa, N, t);
        return { fa.begin(), fa.begin() + N };
    }
}

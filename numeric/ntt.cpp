namespace ntt {
    template<int MOD>
    vector<modnum<MOD>> operator*(vector<modnum<MOD>> a, vector<modnum<MOD>> b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1;
        if (min(sz(a), sz(b)) < 150) {
            const vv_t VV_BOUND = numeric_limits<vv_t>::max() - vv_t(MOD) * MOD;
            vector<vv_t> res(s);
            for (int i = 0; i < sz(a); i++) for (int j = 0; j < sz(b); j++) {
                if ((res[i + j] += vv_t(a[i].v) * b[j].v) > VV_BOUND) res[i + j] %= MOD;
            }
            return {res.begin(), res.end()};
        }

        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);

        bool eq = a == b;
        a.resz(N);
        fft<modnum<MOD>, false>(a, N);

        if (!eq) {
            b.resz(N);
            fft<modnum<MOD>, false>(b, N);
            for (int i = 0; i < N; i++) a[i] *= b[i];
        } else {
            for (int i = 0; i < N; i++) a[i] *= a[i];
        }

        fft<modnum<MOD>, true>(a, N);
        a.resz(s);
        return a;
    }

    template<int MOD> vector<modnum<MOD>> pow(vector<modnum<MOD>> v, int p) {
        vector<modnum<MOD>> r = {1};
        if (!p) return r;
        for (int i = 31 - __builtin_clz(p); i >= 0; --i) {
            r = r * r;
            if (p & (1 << i)) r = r * v;
        }
        return r;
    }
}

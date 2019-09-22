namespace ntt {
    template<int MOD> struct mnum : modnum<MOD> {
        mnum(modnum<MOD> m) : modnum<MOD>(m) {}
        mnum(ll _v = 0) : modnum<MOD>(_v) {}

        static mnum unity_root(int deg, int pow) {
            static vector<mnum> table{ 0, 1 };
            while (sz(table) <= deg) {
                mnum w = modnum<MOD>::unity_root(sz(table));
                for (int i = sz(table)/2, s = sz(table); i < s; i++) {
                    table.push_back(table[i]);
                    table.push_back(table[i] * w);
                }
            }
            return table[deg + (pow < 0 ? deg + pow : pow)];
        }
    };

    template<int MOD>
    vector<modnum<MOD>> operator*(const vector<modnum<MOD>>& a, const vector<modnum<MOD>>& b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1;
        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);

        static vector<mnum<MOD>> fa, fb;
        if (N > sz(fa)) fa.resize(N), fb.resize(N);

        copy(all(a), fa.begin());
        fill(fa.begin() + sz(a), fa.begin() + N, 0);
        copy(all(b), fb.begin());
        fill(fb.begin() + sz(b), fb.begin() + N, 0);

        fft<mnum<MOD>, false>(fa, N);
        fft<mnum<MOD>, false>(fb, N);
        for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
        fft<mnum<MOD>, true>(fa, N);
        return { fa.begin(), fa.begin() + s };
    }
}

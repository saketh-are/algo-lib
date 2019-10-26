namespace complex_fft {
    using dbl = double;
    struct cnum : point<dbl> {
        cnum(point<dbl> p) : point<dbl>(p) {}
        cnum(dbl _x = 0, dbl _y = 0) : point<dbl>(_x, _y) {}
        cnum inv() const { dbl n = norm(); return cnum(x / n, -y / n); }

        static cnum unity_root(int deg, int pow) {
            static vector<cnum> table{ 0, 1 };
            while (sz(table) <= deg) {
                dbl t = 2 * acos(-1) / sz(table);
                cnum w{ cos(t), sin(t) };
                for (int i = sz(table)/2, s = sz(table); i < s; i++) {
                    table.push_back(table[i]);
                    table.push_back(table[i] * w);
                }
            }
            return table[deg + (pow < 0 ? deg + pow : pow)];
        }
    };
    static vector<cnum> fa, fb;

    vector<cnum> operator*(const vector<cnum>& a, const vector<cnum>& b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1;
        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);
        if (N > sz(fa)) fa.resize(N), fb.resize(N);

        copy(all(a), fa.begin());
        fill(fa.begin() + sz(a), fa.begin() + N, 0);
        copy(all(b), fb.begin());
        fill(fb.begin() + sz(b), fb.begin() + N, 0);

        fft<cnum, false>(fa, N);
        fft<cnum, false>(fb, N);
        for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
        fft<cnum, true>(fa, N);
        return { fa.begin(), fa.begin() + s };
    }

    template<int MOD>
    vector<modnum<MOD>> operator*(const vector<modnum<MOD>>& a, const vector<modnum<MOD>>& b) {
        if (a.empty() || b.empty()) return {};
        int s = sz(a) + sz(b) - 1;
        if (min(sz(a), sz(b)) < FFT_CUTOFF) {
            const vv_t VV_BOUND = numeric_limits<vv_t>::max() - vv_t(MOD) * MOD;
            vector<vv_t> res(s);
            for (int i = 0; i < sz(a); i++) for (int j = 0; j < sz(b); j++) {
                if ((res[i + j] += vv_t(a[i].v) * b[j].v) > VV_BOUND) res[i + j] %= MOD;
            }
            return {res.begin(), res.end()};
        }

        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);
        if (N > sz(fa)) fa.resize(N), fb.resize(N);

        for (int i = 0; i < sz(a); i++)
            fa[i] = cnum(a[i].v & ((1 << 15) - 1), a[i].v >> 15);
        fill(fa.begin() + sz(a), fa.begin() + N, 0);
        fft<cnum, false>(fa, N);

        if (a != b) {
            for (int i = 0; i < sz(b); i++)
                fb[i] = cnum(b[i].v & ((1 << 15) - 1), b[i].v >> 15);
            fill(fb.begin() + sz(b), fb.begin() + N, 0);
            fft<cnum, false>(fb, N);
        } else {
            copy(fa.begin(), fa.begin() + N, fb.begin());
        }

        for (int i = 0; i <= N / 2; i++) {
            int j = (N - i) & (N - 1);
            cnum g0 = (fb[i] + fb[j].conj()) / (2 * N);
            cnum g1 = (fb[i] - fb[j].conj()) / (2 * N);
            g1 = { g1.y, -g1.x };
            if (j != i) {
                swap(fa[j], fa[i]);
                fb[j] = fa[j] * g1;
                fa[j] = fa[j] * g0;
            }
            fb[i] = fa[i] * g1.conj();
            fa[i] = fa[i] * g0.conj();
        }
        fft<cnum, false>(fa, N), fft<cnum, false>(fb, N);

        vector<modnum<MOD>> c(s);
        for (int i = 0; i < s; i++)
            c[i] =  ll(fa[i].x + 0.5)
                 + (ll(fa[i].y + 0.5) % MOD << 15)
                 + (ll(fb[i].x + 0.5) % MOD << 15)
                 + (ll(fb[i].y + 0.5) % MOD << 30);
        return c;
    }
    template<int MOD>
    vector<modnum<MOD>> conv(const vector<modnum<MOD>>& a, const vector<modnum<MOD>>& b) {
        return a * b;
    }
}

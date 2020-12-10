// {{{ numeric/modnum.cpp }}}
// {{{ numeric/point.cpp }}}
// {{{ numeric/fft.cpp }}}

#include <vector>
#include <climits>

namespace complex_fft {
    using dbl = double;
    struct cnum : point<dbl> {
        cnum(point<dbl> p) : point<dbl>(p) {}
        cnum(dbl _x = 0, dbl _y = 0) : point<dbl>(_x, _y) {}
        cnum inv() const { dbl n = norm(); return cnum(x / n, -y / n); }

        static cnum unity_root(int deg, int pow) {
            static std::vector<cnum> table{ 0, 1 };
            while (int(table.size()) <= deg) {
                dbl t = 2 * acos(-1) / table.size();
                cnum w{ cos(t), sin(t) };
                for (int s = int(table.size()), i = s / 2; i < s; i++) {
                    table.push_back(table[i]);
                    table.push_back(table[i] * w);
                }
            }
            return table[deg + (pow < 0 ? deg + pow : pow)];
        }
    };

    static std::vector<cnum> fa, fb;

    std::vector<cnum> operator*(const std::vector<cnum>& a, const std::vector<cnum>& b) {
        if (a.empty() || b.empty())
            return {};

        int s = int(a.size()) + int(b.size()) - 1;

        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);
        if (N > int(fa.size())) fa.resize(N), fb.resize(N);

        copy(a.begin(), a.end(), fa.begin());
        fill(fa.begin() + a.size(), fa.begin() + N, 0);
        copy(b.begin(), b.end(), fb.begin());
        fill(fb.begin() + b.size(), fb.begin() + N, 0);

        fft(fa.begin(), fa.begin() + N, false);
        fft(fb.begin(), fb.begin() + N, false);
        for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
        fft(fa.begin(), fa.begin() + N, true);

        return { fa.begin(), fa.begin() + s };
    }

    template<int MOD>
    std::vector<modnum<MOD>> operator*(const std::vector<modnum<MOD>>& a, const std::vector<modnum<MOD>>& b) {
        if (a.empty() || b.empty())
            return {};

        int s = int(a.size()) + int(b.size()) - 1;

        if (std::min(a.size(), b.size()) < FFT_CUTOFF) {
            const vv_t VV_BOUND = std::numeric_limits<vv_t>::max() - vv_t(MOD) * MOD;
            std::vector<vv_t> res(s);
            for (int i = 0; i < int(a.size()); i++) {
                for (int j = 0; j < int(b.size()); j++) {
                    res[i + j] += vv_t(a[i].v) * b[j].v;
                    if (res[i + j] > VV_BOUND)
                        res[i + j] %= MOD;
                }
            }
            return {res.begin(), res.end()};
        }

        int N = 1 << (s > 1 ? 32 - __builtin_clz(s - 1) : 0);
        if (N > int(fa.size())) fa.resize(N), fb.resize(N);

        for (int i = 0; i < int(a.size()); i++)
            fa[i] = cnum(a[i].v & ((1 << 15) - 1), a[i].v >> 15);
        fill(fa.begin() + a.size(), fa.begin() + N, 0);
        fft(fa.begin(), fa.begin() + N, false);

        if (a != b) {
            for (int i = 0; i < int(b.size()); i++)
                fb[i] = cnum(b[i].v & ((1 << 15) - 1), b[i].v >> 15);
            fill(fb.begin() + b.size(), fb.begin() + N, 0);
            fft(fb.begin(), fb.begin() + N, false);
        } else {
            copy(fa.begin(), fa.begin() + N, fb.begin());
        }

        for (int i = 0; i <= N / 2; i++) {
            int j = (N - i) & (N - 1);
            cnum g0 = (fb[i] + fb[j].conj()) / (2 * N);
            cnum g1 = (fb[i] - fb[j].conj()) / (2 * N);
            g1 = { g1.y, -g1.x };
            if (j != i) {
                std::swap(fa[j], fa[i]);
                fb[j] = fa[j] * g1;
                fa[j] = fa[j] * g0;
            }
            fb[i] = fa[i] * g1.conj();
            fa[i] = fa[i] * g0.conj();
        }

        fft(fa.begin(), fa.begin() + N, false);
        fft(fb.begin(), fb.begin() + N, false);

        std::vector<modnum<MOD>> c(s);
        for (int i = 0; i < s; i++)
            c[i] =  vv_t(fa[i].x + 0.5)
                 + (vv_t(fa[i].y + 0.5) % MOD << 15)
                 + (vv_t(fb[i].x + 0.5) % MOD << 15)
                 + (vv_t(fb[i].y + 0.5) % MOD << 30);
        return c;
    }
}

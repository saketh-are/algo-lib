// {{{ numeric/modnum.cpp }}}
// {{{ numeric/fft.cpp }}}

#include <vector>
#include <climits>

namespace ntt {
    template<int MOD>
    std::vector<modnum<MOD>> operator*(std::vector<modnum<MOD>> a, std::vector<modnum<MOD>> b) {
        if (a.empty() || b.empty()) return {};
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

        bool eq = a == b;
        a.resize(N);
        fft(a.begin(), a.end(), false);

        if (!eq) {
            b.resize(N);
            fft(b.begin(), b.end(), false);
            for (int i = 0; i < N; i++) a[i] *= b[i];
        } else {
            for (int i = 0; i < N; i++) a[i] *= a[i];
        }

        fft(a.begin(), a.end(), true);
        a.resize(s);
        return a;
    }

    template<int MOD>
    std::vector<modnum<MOD>> pow(std::vector<modnum<MOD>> v, int p) {
        std::vector<modnum<MOD>> r = {1};
        if (!p) return r;
        for (int i = 31 - __builtin_clz(p); i >= 0; --i) {
            r = r * r;
            if (p & (1 << i)) r = r * v;
        }
        return r;
    }
}

template<typename H, int B, typename V> struct rabin_karp {
    array<H, B> bases;
    vector<H> pows;

    rabin_karp(int maxv) {
        for (int i = 0; i < B; i++)
            bases[i] = H(maxv + rng() % maxv);
        pows.resize(B, H(1));
    }
    H pow(int i, int e) {
        while (pows.size() <= B * e + i)
            pows.push_back(pows[pows.size() - B] * bases[pows.size() % B]);
        return pows[B * e + i];
    }

    struct polyhash {
        rabin_karp<H, B, V>& rk;
        vector<H> pref;

        polyhash(const vector<V>& e, auto& _rk) : rk(_rk) {
            pref.resize(B * (e.size() + 1));
            for (int i = 0; i < pref.size(); i++)
                if (i < B) pref[i] = H(0);
                else pref[i] = pref[i - B] * rk.bases[i % B] + H(e[i/B - 1]);
        }

        array<H, B> operator () (int i, int j) const {
            array<H, B> res;
            for (int b = 0; b < B; b++)
                res[b] = pref[j * B + b] - rk.pow(b, j - i) * pref[i * B + b];
            return res;
        }
    };
    polyhash hash(const vector<V>& e) { return polyhash(e, *this); }
};


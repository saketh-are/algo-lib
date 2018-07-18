namespace rabin_karp {
    const int NPRIME = 3;
    const int P1 = 1e9+7, P2 = 1e9+9, P3 = 1e9+21;
    const int primes[3] = { P1, P2, P3 };

    struct strong_mod {
        int vals[NPRIME];

        strong_mod(ll value = 0) {
            for (int i = 0; i < NPRIME; i++) {
                vals[i] = value % primes[i];
                if(vals[i] < 0) vals[i] += primes[i];
            }
        }

        #define combine(h, op) \
            strong_mod res; \
            for (int i = 0; i < NPRIME; i++) \
                res.vals[i] = (vals[i] op h.vals[i]) % primes[i]; \
            return res;

        bool operator==(const strong_mod &h) { return !memcmp(this, &h, sizeof(strong_mod)); }
        strong_mod operator*(const strong_mod &h) { combine(h, * 1ll *); }
        strong_mod operator+(const strong_mod &h) { combine(h, +); }

        strong_mod operator*(ll m) { return (*this) * strong_mod(m); }
        strong_mod operator+(ll a) { return (*this) + strong_mod(a); }
    };

    const int MAXL = 2e5 + 200;
    const int BASE = 3001;

    vector<strong_mod> powers;

    void init() {
        powers.resize(MAXL);
        powers[0] = strong_mod(1);
        for(int i=1; i<MAXL; i++)
            powers[i] = powers[i-1] * BASE;
    }

    void compute(vector<strong_mod> &hashes, string str) {
        hashes.resize(str.size() + 1);
        for(int i=1; i<=str.size(); i++)
            hashes[i] = hashes[i-1] * BASE + int(str[i-1]);
    }

    strong_mod read(vector<strong_mod> &hashes, int i, int l) {
        strong_mod sub = hashes[i] * powers[l];
        return hashes[i+l] + (sub * -1);
    }
};


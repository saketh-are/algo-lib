template<typename T> struct LFSR {
    vector<T> seq;    // original sequence
    vector<T> fib;    // seq[i] = sum_j fib[j] * seq[i-1-j] (i >= sz(fib))
    vector<T> galois; // 0 = sum_j galois[j] * seq[i+j] (i <= sz(seq) - sz(galois))

    LFSR(vector<T> _seq) : seq(_seq) {
        vector<T> C = {1}, B = {1}; T b = 1;
        for (int i = 0, m = 1; i < sz(seq); i++, m++) {
            T d = 0; for (int j = 0; j < sz(C); j++) d += C[j] * seq[i - j];
            if (d == 0) continue;
            vector<T> _B = C; C.resz(max(sz(C), m+sz(B)));
            T dbi = d/b; for (int j = 0; j < sz(B); j++) C[m+j] -= dbi * B[j];
            if (sz(_B) < m+sz(B)) { B = _B; b = d; m = 0; }
        }
        galois = rev(C);
        fib = move(C); fib.erase(fib.begin()); fib *= T{-1};
    }

    T operator[](ll n) const {
        vector<T> r = dot(seq, mod_pow({0,1}, n, galois));
        return accumulate(all(r), T{0});
    }
};

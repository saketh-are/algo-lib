namespace vector_operators {
    template<typename T> vector<T>& operator*=(vector<T>& l, const T& r) {
        for (T& e : l) e *= r; return l;
    }
    template<typename T> vector<T>& operator/=(vector<T>& l, const T& r) {
        for (T& e : l) e /= r; return l;
    }
    template<typename T> vector<T> operator*(vector<T> l, const T& r) { return l *= r; }
    template<typename T> vector<T> operator*(const T& l, vector<T> r) { return r *= l; }
    template<typename T> vector<T> operator/(vector<T> l, const T& r) { return l /= r; }

    template<typename T> vector<T>& operator+=(vector<T>& l, vector<T>& r) {
        l.resz(max(sz(l), sz(r))); for (int i = 0; i < sz(r); i++) l[i] += r[i]; return l;
    }
    template<typename T> vector<T>& operator-=(vector<T>& l, const vector<T>& r) {
        l.resz(max(sz(l), sz(r))); for (int i = 0; i < sz(r); i++) l[i] -= r[i]; return l;
    }
    template<typename T> vector<T> operator+(vector<T> l, const vector<T>& r) { return l += r; }
    template<typename T> vector<T> operator-(vector<T> l, const vector<T>& r) { return l -= r; }

    template<typename T> vector<T> conv(const vector<T>& l, const vector<T>& r) {
        if (l.empty() || r.empty()) return {};
        vector<T> p(sz(l) + sz(r) - 1);
        for (int i = 0; i < sz(l); i++) for (int j = 0; j < sz(r); j++) p[i+j] += l[i] * r[j];
        return p;
    }
    template<typename T> vector<T> operator*(const vector<T>& l, const vector<T>& r) {
        return complex_fft::conv(l, r); // return conv(l, r);
    }

    template<typename T> vector<T> dot(const vector<T>& a, const vector<T>& b) {
        vector<T> p(min(sz(a), sz(b))); for (int i = 0; i < sz(p); i++) p[i] = a[i] * b[i]; return p;
    }

    template<typename T> vector<T>& operator<<=(vector<T>& v, int x) {
        v.insert(v.begin(), x, 0); return v;
    }
    template<typename T> vector<T> operator<<(vector<T> v, int x) { return v <<= x; }

    template<typename T> void rem_lead(vector<T>& v) { while (sz(v) && v.back() == 0) v.pop_back(); }
    template<typename T> vector<T> without_lead(vector<T> v) { rem_lead(v); return v; }

    template<typename T> vector<T> reversed(vector<T> v) {
        reverse(all(v)); rem_lead(v); return v;
    }
    template<typename T> vector<T> reversed(const vector<T>& v, int S) {
        assert(S >= sz(v)); vector<T> res(S); copy(all(v), res.rbegin()); return res;
    }

    template<typename T> pair<vector<T>, vector<T>> long_division(vector<T> a, vector<T> b) {
        rem_lead(a), rem_lead(b), assert(!b.empty());
        T B = T{1} / b.back(); for (T& e : b) e *= B;
        vector<T> q(max(sz(a) - sz(b) + 1, 0));
        for (; sz(a) >= sz(b); rem_lead(a)) {
            q[sz(a) - sz(b)] = a.back();
            a -= a.back() * (b << (sz(a) - sz(b)));
        }
        for (T& e : q) e *= B;
        return {q, a}; // quotient, remainder
    }

    template<typename T> vector<T> mod_xk(const vector<T>& v, int k) {
        vector<T> r{v.begin(), v.begin() + min(sz(v), k)}; rem_lead(r); return r;
    }
    // 1/v % x^k
    template<typename T> vector<T> inv(const vector<T>& v, int k) {
        assert(!v.empty());
        if (k == 0) return {};
        if (k == 1) return { T{1}/v[0] };
        vector<T> v0 = inv(v, (k + 1)/2);
        return mod_xk(v0 * (vector<T>{2} - v0 * mod_xk(v, k)), k);
    }
    // Fast quotient and remainder by multiplying with inverse if you have an FFT
    template<typename T> vector<T> quo(vector<T> a, vector<T> b) {
        if (sz(a) < sz(b)) return {0};
        rem_lead(a), rem_lead(b), assert(!b.empty());
        int s = sz(a) + 1 - sz(b);
        return reversed(mod_xk(reversed(a) * inv(reversed(b), s), s), s);
    }
    template<typename T> vector<T> rem(const vector<T>& a, const vector<T>& b) {
        return without_lead(sz(a) < sz(b) ? a : (a - quo(a, b) * b));
    }

    template<typename T> vector<T>& operator/=(vector<T>& a, const vector<T>& b) {
        return a = min(sz(a), sz(b)) < FFT_CUTOFF ? long_division(a, b).first  : quo(a, b);
    }
    template<typename T> vector<T>& operator%=(vector<T>& a, const vector<T>& b) {
        return a = min(sz(a), sz(b)) < FFT_CUTOFF ? long_division(a, b).second : rem(a, b);
    }
    template<typename T> vector<T> operator/(vector<T> a, const vector<T>& b) { return a /= b; }
    template<typename T> vector<T> operator%(vector<T> a, const vector<T>& b) { return a %= b; }

    template<typename T> vector<T> mod_pow(const vector<T>& v, ll e, const vector<T>& m) {
        if (!e) return {1};
        vector<T> res = mod_pow(v, e/2, m); res = res * res % m;
        return e&1 ? res * v % m : res;
    }

    // evaluate polynomial v at x0
    template<typename T> T operator|(const vector<T>& v, const T& x0) {
        T res = 0; for (int i = sz(v) - 1; i >= 0; --i) res = res * x0 + v[i]; return res;
    }
}

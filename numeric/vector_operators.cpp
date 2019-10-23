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

    // discrete convolution
    template<typename T> vector<T> operator*(const vector<T>& l, const vector<T>& r) {
        if (l.empty() || r.empty()) return {};
        vector<T> p(sz(l) + sz(r) - 1);
        for (int i = 0; i < sz(l); i++) for (int j = 0; j < sz(r); j++) p[i+j] += l[i] * r[j];
        return p;
    }
    template<typename T> vector<T>& operator*=(vector<T>& l, const vector<T>& r) { return l = l * r; }

    // dot product
    template<typename T> vector<T> dot(const vector<T>& a, const vector<T>& b) {
        vector<T> p(min(sz(a), sz(b))); for (int i = 0; i < sz(p); i++) p[i] = a[i] * b[i]; return p;
    }

    template<typename T> vector<T>& operator<<=(vector<T>& v, int x) {
        v.insert(v.begin(), x, 0); return v;
    }
    template<typename T> vector<T> operator<<(vector<T> v, int x) { return v <<= x; }

    template<typename T> vector<T> rev(vector<T> v) { reverse(all(v)); return v; }
    template<typename T> void rem_lead(vector<T>& v) { while (sz(v) && v.back() == 0) v.pop_back(); }

    template<typename T> pair<vector<T>, vector<T>> div(vector<T> a, vector<T> b) {
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
    template<typename T> vector<T>& operator/=(vector<T>& a, const vector<T>& b) {
        return a = div(a,b).first;
    }
    template<typename T> vector<T>& operator%=(vector<T>& a, const vector<T>& b) {
        return a = div(a,b).second;
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

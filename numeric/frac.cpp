struct frac {
    ll n, d;
    frac(ll _n, ll _d) : n(_n), d(_d) {
        assert(n != 0 || d != 0);
        if (d < 0) { n *= -1; d *= -1; }
        ll g = __gcd(abs(n), d);
        n /= g;
        d /= g;
    }
    friend ostream& operator << (ostream& o, const frac& f) {
        return o << f.n << "/" << f.d;
    }

    friend bool operator < (const frac& a, const frac& b) {
        return a.n * b.d < b.n * a.d;
    }
    friend bool operator <= (const frac& a, const frac& b) {
        return a.n * b.d <= b.n * a.d;
    }
    friend bool operator == (const frac& a, const frac& b) {
        return a.n * b.d == b.n * a.d;
    }
    friend bool operator >= (const frac& a, const frac& b) {
        return a.n * b.d >= b.n * a.d;
    }
    friend bool operator > (const frac& a, const frac& b) {
        return a.n * b.d > b.n * a.d;
    }
    friend frac min(const frac a, const frac b) { return a <= b ? a : b; }
    friend frac max(const frac a, const frac b) { return a >= b ? a : b; }

    // canonical continued fraction
    vll to_cont() const {
        if (d == 0) return { LLONG_MAX };
        vll cont;
        for (ll n = this->n, d = this->d; ; swap(n, d)) {
            ll f = (n >= 0 ? n : n - d + 1) / d;
            cont.push_back(f);
            n -= f * d;
            if (n == 0) break;
        }
        return cont;
    }

    static frac from_cont(const vll& cont) {
        ll n = 1, d = 0;
        for (int i = int(cont.size()) - 1; i >= 0; i--) {
            swap(n, d);
            n += d * cont[i];
        }
        return { n, d };
    }

    // "best" fraction in (x, y): minimizes both n and d
    friend frac best_in(const frac x, const frac y) {
        assert(x < y);

        vll x1 = x.to_cont(), y1 = y.to_cont();
        vll x2 = x1; x2.back()--; x2.push_back(1);
        vll y2 = y1; y2.back()--; y2.push_back(1);

        auto z = [](const vll& a, const vll& b) {
            vll c;
            for (int i = 0; ; i++) {
                ll ai = i < a.size() ? a[i] : LLONG_MAX;
                ll bi = i < b.size() ? b[i] : LLONG_MAX;
                if (ai != bi) {
                    c.push_back(min(ai, bi) + 1);
                    return from_cont(c);
                }
                c.push_back(ai);
            }
        };

        frac ans = { 1, 0 };
        for (const frac f : {z(x1, y1), z(x1, y2), z(x2, y1), z(x2, y2)}) {
            if (x < f && f < y && (ans.d == 0 || f.d < ans.d))
                ans = f;
        }
        return ans;
    }
};

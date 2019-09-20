struct dbl {
    static constexpr double EPS = 1e-9;

    double d;
    dbl() : d(0) {}
    dbl(double _d) : d(_d) {}
    explicit operator double() { return d; }
    friend istream& operator >> (istream& i, dbl& n) { double v; i >> v; n = dbl(v); return i; }
    friend ostream& operator << (ostream& o, const dbl& n) { return o << n.d; }

    dbl& operator += (const dbl& o) { d += o.d; return *this; }
    dbl& operator -= (const dbl& o) { d -= o.d; return *this; }
    dbl& operator *= (const dbl& o) { d *= o.d; return *this; }
    dbl& operator /= (const dbl& o) { d /= o.d; return *this; }
    friend dbl operator + (const dbl& a, const dbl& b) { return dbl(a) += b; }
    friend dbl operator - (const dbl& a, const dbl& b) { return dbl(a) -= b; }
    friend dbl operator * (const dbl& a, const dbl& b) { return dbl(a) *= b; }
    friend dbl operator / (const dbl& a, const dbl& b) { return dbl(a) /= b; }

    bool operator == (const dbl& o) const { return abs(d - o.d) < EPS; }
    bool operator != (const dbl& o) const { return abs(d - o.d) >= EPS; }
    bool operator < (const dbl& o) const { return d < o.d; }
    bool operator > (const dbl& o) const { return d > o.d; }
    bool operator <= (const dbl& o) const { return d < o.d + EPS; }
    bool operator >= (const dbl& o) const { return d + EPS > o.d; }

    friend double sqrt(dbl v){ return sqrt(v.d); }
};

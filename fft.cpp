template<typename T> struct FFT {
    int deg;
    vector<int> rev;

    T deg_inv;

    FFT(int _deg, T _deg_inv) {
        assert(__builtin_popcount(_deg) == 1);
        deg = _deg;
        deg_inv = _deg_inv;
        rev.resize(deg);

        for (int i = 1, j = 0; i < deg; i++) {
            int bit = deg / 2;
            for (; j >= bit; bit /= 2)
                j -= bit;
            j += bit;
            rev[i] = j;
        }
    }

    void transform(vector<T> &poly, bool invert) {
        assert(poly.size() == deg);

        for (int i = 1; i < deg; i++)
            if(rev[i] > i)
                swap(poly[i], poly[rev[i]]);

        for (int len = 2; len <= deg; len *= 2) {
            T base = T::unity_root(deg, deg/len * (invert ? -1 : 1));

            for (int i = 0; i < deg; i += len) {
                T w = T::one();

                for (int j = 0; j < len / 2; j++) {
                    T u = poly[i+j];
                    T v = w * poly[i+j+len/2];

                    poly[i+j] = u + v;
                    poly[i+j+len/2] = u - v;

                    w = w * base;
                }
            }
        }

        if(invert) {
            for (int i = 0; i < deg; i++)
                poly[i] = poly[i] * deg_inv;
        }
    }
};

struct cnum {
    static constexpr double PI = 2 * atan2(1, 0);
    static constexpr cnum one() { return { 1, 0 }; }

    double a, b;
    cnum operator+(const cnum &c) { return { a + c.a, b + c.b }; }
    cnum operator-(const cnum &c) { return { a - c.a, b - c.b }; }
    cnum operator*(const cnum &c) { return { a*c.a - b*c.b, a*c.b + b*c.a }; }

    // returns x^pow for x such that x^deg = 1, x^(deg-1) != 1
    static cnum unity_root(int deg, int pow) {
        double ang = pow * 2 * PI / deg;
        return { cos(ang), sin(ang) };
    }
};


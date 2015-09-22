namespace fft {
    struct cnum {
        double a, b;
        cnum operator+(const cnum &c) { return { a + c.a, b + c.b }; }
        cnum operator-(const cnum &c) { return { a - c.a, b - c.b }; }
        cnum operator*(const cnum &c) { return { a*c.a - b*c.b, a*c.b + b*c.a }; }
        cnum operator/(double d) { return { a / d, b / d }; }
    };

    const double PI = 2 * atan2(1, 0);

    int deg;
    vector<int> rev;

    void set_degree(int _deg) {
        assert(__builtin_popcount(_deg) == 1);
        deg = _deg;
        rev.resize(deg);

        for (int i = 1, j = 0; i < deg; i++) {
            int bit = deg / 2;
            for (; j >= bit; bit /= 2)
                j -= bit;
            j += bit;

            rev[i] = j;
        }
    }

    void transform(vector<cnum> &poly, bool invert) {
        if(deg != poly.size()) set_degree(poly.size());

        for (int i = 1; i < deg; i++)
            if(rev[i] > i)
                swap(poly[i], poly[rev[i]]);

        for (int len = 2; len <= deg; len *= 2) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cnum base = { cos(ang), sin(ang) };

            for (int i = 0; i < deg; i += len) {
                cnum w = {1, 0};

                for (int j = 0; j < len / 2; j++) {
                    cnum u = poly[i+j];
                    cnum v = w * poly[i+j+len/2];

                    poly[i+j] = u + v;
                    poly[i+j+len/2] = u - v;

                    w = w * base;
                }
            }
        }

        if(invert) {
            for (int i = 0; i < deg; i++)
                poly[i] = poly[i] / double(deg);
        }
    }
};


template<typename V, int CT>
struct polynomial_hash {
    static const int SIGMA = 256;

    static V pow(int xi, int e) {
        static vector<V> x(CT, 1);
        while (int(x.size()) < 2 * CT)
            x.push_back(rng() % SIGMA + SIGMA);
        while (int(x.size()) <= e * CT + xi)
            for (int i = 0; i < CT; i++)
                x.push_back(*(x.end() - CT) * x[CT + i]);
        return x[e * CT + xi];
    }

    int N;
    array<V, CT> data;
    polynomial_hash () : N(0) { data.fill(0); }
    polynomial_hash (V v) : N(1) { assert(v.v < SIGMA); data.fill(v); }
    polynomial_hash (int N_, array<V, CT> data_) : N(N_), data(data_) {}

    polynomial_hash operator+(const polynomial_hash &o) const {
        array<V, CT> res;
        for (int i = 0; i < CT; i++)
            res[i] = data[i] * pow(i, o.N) + o.data[i];
        return polynomial_hash { N + o.N, res };
    }
    bool operator==(const polynomial_hash &o) const {
        return N == o.N && data == o.data;
    }
    friend ostream& operator << (ostream& o, const polynomial_hash& h) {
        o << h.N;
        for (int i = 0; i < CT; i++) o << " " << h.data[i];
        return o;
    }

    template<typename I>
    static vector<polynomial_hash<V, CT>> get_prefixes(I begin, I end) {
        vector<polynomial_hash<V, CT>> res(1);
        for (I iter = begin; iter != end; iter = next(iter))
            res.push_back(res.back() + polynomial_hash<V, CT>(V(*iter)));
        return res;
    }

    static polynomial_hash get_substr(const vector<polynomial_hash> &prefixes, int pos, int len) {
        array<V, CT> res;
        for (int i = 0; i < CT; i++)
            res[i] = prefixes[pos + len].data[i] - pow(i, len) * prefixes[pos].data[i];
        return polynomial_hash { len, res };
    }
};

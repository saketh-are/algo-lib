template<typename V, int CT, int SIGMA>
struct polynomial_hash {
    static V pow(int xi, int e) {
        static vector<V> x(CT, 1);
        if (int(x.size()) == CT) {
            x.push_back(SIGMA);
            for (int i = 1; i < CT; i++)
                x.push_back(rng() % max(256, SIGMA) + SIGMA);
        }
        while (e * CT + xi >= int(x.size()))
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
    static vector<polynomial_hash> get_prefixes(I begin, I end) {
        vector<polynomial_hash> res(1);
        for (I iter = begin; iter != end; iter = next(iter))
            res.push_back(res.back() + polynomial_hash(V(*iter)));
        return res;
    }

    static polynomial_hash get_substr(const vector<polynomial_hash> &prefixes, int pos, int len) {
        array<V, CT> res;
        for (int i = 0; i < CT; i++)
            res[i] = prefixes[pos + len].data[i] - pow(i, len) * prefixes[pos].data[i];
        return polynomial_hash { len, res };
    }
};

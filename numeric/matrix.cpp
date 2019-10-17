template<typename T> struct matrix {
    int N, M;
    T* data;

    matrix<T> (int _N, int _M, T f = 0, T d = 0) : N(_N), M(_M) {
        data = (T*) malloc(N * M * sizeof(T));
        fill(data, data + N * M, f);
        for (int i = 0; i < min(N, M); i++)
            (*this)[i][i] = d;
    }

    matrix<T> (int _N, int _M, const initializer_list<T>& init) : N(_N), M(_M) {
        assert(sz(init) == N * M);
        data = (T*) malloc(N * M * sizeof(T));
        copy(all(init), data);
    }

    matrix<T> (const matrix<T>& m) : N(m.N), M(m.M) {
        data = (T*) malloc(N * M * sizeof(T));
        copy(m.data, m.data + N * M, data);
    }

    matrix<T>& operator=(const matrix<T>& o) {
        if (this != &o) {
            N = o.N, M = o.M;
            free(data);
            data = (T*) malloc(N * M * sizeof(T));
            copy(o.data, o.data + N * M, data);
        }
        return *this;
    }

    ~matrix<T> () { free(data); }

    explicit operator T() const { assert(N == 1 && M == 1); return data[0]; }
    T* operator[](int i) { return data + i * M; }
    const T* operator[](int i) const { return data + i * M; }

    friend matrix<T> operator *(const matrix<T>& a, const matrix<T>& b) {
        assert(a.M == b.N);
        matrix<T> res(a.N, b.M);
        for (int i = 0; i < a.N; i++)
            for (int j = 0; j < b.M; j++)
                for (int k = 0; k < a.M; k++)
                    res[i][j] += a[i][k] * b[k][j];
        return res;
    }

    friend vector<T> operator *(const vector<T>& v, const matrix<T>& m) {
        assert(sz(v) == m.N);
        vector<T> res(m.M);
        for (int j = 0; j < m.M; j++)
            for (int i = 0; i < m.N; i++)
                res[j] += v[i] * m[i][j];
        return res;
    }

    friend vector<T> operator *(const matrix<T>& m, const vector<T>& v) {
        assert(m.M == sz(v));
        vector<T> res(m.N);
        for (int i = 0; i < m.N; i++)
            for (int j = 0; j < m.M; j++)
                res[i] += m[i][j] * v[j];
        return res;
    }

    matrix pow(ll e) const {
        assert(N == M);
        if (e == 0) return matrix<T>(N, M, 0, 1);
        if (e&1) return *this * pow(e - 1);
        return (*this * *this).pow(e / 2);
    }

    int rank() const {
        matrix<T> m = *this;
        int r = 0;
        for (int j = 0; j < M; j++) {
            for (int i = r; i < N; i++) if (m[i][j] != 0) {
                swap_ranges(m[r], m[r] + M, m[i]);
                break;
            }
            if (m[r][j] == 0) continue;

            for (int i = 0; i < N; i++) if (i != r) {
                T c = m[i][j] / m[r][j];
                for (int k = 0; k < M; k++)
                    m[i][k] -= c * m[r][k];
            }
            r++;
        }
        return r;
    }

    friend ostream& operator <<(ostream& os, const matrix<T>& m) {
        for (int i = 0; i < m.N; i++) {
            os << (i ? i < m.N - 1 ? "\u2503" : "\u2517" : "\n\u250F");
            for (int j = 0; j < m.M; j++)
                os << setw(12) << m[i][j];
            os << "  " << (i ? i < m.N - 1 ? "\u2503" : "\u251B" : "\u2512") << "\n";
        }
        return os;
    }
};

template<typename T> struct matrix {
    int N;
    vector<T> dat;

    matrix<T> (int _N, T fill = T(0), T diag = T(1)) : N(_N) {
        dat.resize(N * N, fill);
        for (int i = 0; i < N; i++)
            (*this)(i, i) = diag;
    }

    T& operator()(int i, int j) {
        return dat[N * i + j];
    }

    matrix<T> operator *(const matrix<T>& b) const {
        matrix<T> r(N);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    r(i, j) += (*this)(i, k) * b(k, j);
        return r;
    }

    matrix<T> pow(ll e) {
        if(!e) return matrix<T>(N);
        if (e&1) return *this * *this.pow(e - 1);
        return (*this * *this).pow(e/2);
    }

    int rank() const {
        matrix<T> m = *this;
        int r = 0;
        for (int i = 0; i < N; i++) {
            for (int j = r; j < N; j++) {
                if (m(j, i) != T(0)) {
                    for (int k = 0; k < N; k++)
                        swap(m(r, k), m(j, k));
                    break;
                }
            }
            if (m(r, i) == T(0)) continue;
            for (int j = 0; j < N; j++) {
                if (j == r) continue;
                T c = m(j, i) / m(r, i);
                for (int k = 0; k < N; k++)
                    m(j, k) -= c * m(r, k);
            }
            r++;
        }
        return r;
    }

    friend ostream& operator<<(ostream& os, const matrix<T>& m) {
        os << "{";
        for (int i = 0; i < m.N; i++) {
            if(i) os << "},\n ";
            os << "{";
            for (int j = 0; j < m.N; j++) {
                if(j) os << ", ";
                os << setw(10) << m(i, j) << setw(0);
            }
        }
        return os << "}}";
    }
};

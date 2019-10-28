/* Equivalent to matrix.cpp, but with data stored in std::array.
 * Useful when matrix dimensions are known at compile time
 * and you want locality for a vector<matrix>. */
template<typename T, int N, int M> struct matrix : array<array<T,M>,N> {
    matrix<T,N,M> (T f = 0, T d = 0) {
        for (array<T,M>& row : *this) row.fill(f);
        for (int i = 0; i < min(N, M); i++) ((*this)[i])[i] = d;
    }

    matrix<T,N,M> (const initializer_list<T>& init) {
        assert(sz(init) == N * M);
        auto it = init.begin();
        for (array<T,M>& row : *this) {
            copy(it, it + M, row.data());
            it += M;
        }
    }

    template<int P>
    friend matrix<T,N,M> operator*(const matrix<T,N,P>& a, const matrix<T,P,M>& b) {
        matrix<T,N,M> res{};
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                for (int k = 0; k < M; k++)
                    res[i][j] += a[i][k] * b[k][j];
        return res;
    }

    friend vector<T> operator*(const vector<T>& v, const matrix<T,N,M>& m) {
        assert(N == sz(v));
        vector<T> res(M);
        for (int j = 0; j < M; j++)
            for (int i = 0; i < N; i++)
                res[j] += v[i] * m[i][j];
        return res;
    }

    friend vector<T> operator*(const matrix<T,N,M>& m, const vector<T>& v) {
        assert(M == sz(v));
        vector<T> res(N);
        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                res[i] += m[i][j] * v[j];
        return res;
    }

    matrix pow(ll e) const {
        assert(N == M);
        if (e == 0) return matrix<T,N,M>(N, M, 0, 1);
        if (e&1) return *this * pow(e - 1);
        return (*this * *this).pow(e / 2);
    }

    int rank() const {
        matrix<T,N,M> m = *this;
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

    friend ostream& operator <<(ostream& os, const matrix<T,N,M>& m) {
        for (int i = 0; i < N; i++) {
            os << (i ? i < N - 1 ? "\u2503" : "\u2517" : "\n\u250F");
            for (int j = 0; j < M; j++)
                os << setw(12) << m[i][j];
            os << "  " << (i ? i < m.N - 1 ? "\u2503" : "\u251B" : "\u2512") << "\n";
        }
        return os;
    }
};

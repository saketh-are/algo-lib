// Naive O(AB) multiplication of polynomials a and b
template<typename T>
T* naive_product(T* c, const T* ai, const T* aj, const T* bi, const T* bj) {
    T* ce = c + (aj - ai) + (bj - bi) - 1;
    fill(c, ce, T{});
    for (auto a = ai; a < aj; a++)
        for (auto b = bi; b < bj; b++)
            c[a - ai + b - bi] += *a * *b;
    return ce;
}

template<typename T>
vector<T> naive_product(const vector<T>& a, const vector<T>& b) {
    vector<T> c(sz(a) + sz(b) - 1);
    naive_product(c.data(), a.data(), a.data() + sz(a), b.data(), b.data() + sz(b));
    return c;
}

template<typename T> T get(T* i, T* j, int k) {
    return i + k < j ? *(i + k) : T{};
}

// O(max(A, B)^log_2(3)) multiplication of polynomials a and b
template<typename T, int SZ>
T* karatsuba(T* c, const T* ai, const T* aj, const T* bi, const T* bj) {
    if (ai >= aj || bi >= bj) return c;
    if (aj - ai < SZ) return naive_product(c, ai, aj, bi, bj);
    assert(aj - ai >= bj - bi);

    const int N = aj - ai + bj - bi - 1;
    const int M = (aj - ai + 1) / 2;
    const int S = max(N, 3 * M);

    for (int i = 0; i < M; i++) {
        c[0 + i]      =  get(ai, aj, i) + get(ai, aj, M + i);
        c[5 * M + i]  =  get(bi, bj, i) + get(bi, bj, M + i);
    }
    karatsuba<T, SZ>(c + M, c, c + M, c + 5 * M, c + 6 * M);
    c[3 * M - 1] = T{};

    T* ce = karatsuba<T, SZ>(c + S, ai, ai + M, bi, min(bi + M, bj));
    for (int i = 0; i < N; i++) {
        if      (i < M)         c[i]  = get(c + S, ce, i);
        else if (i < 2 * M)     c[i] += get(c + S, ce, i) - get(c + S, ce, i - M);
        else                    c[i] -= get(c + S, ce, i - M);
    }

    ce = karatsuba<T, SZ>(c + S, ai + M, aj, bi + M, bj);
    for (int i = M; i < N; i++) {
        if      (i < 2 * M)     c[i] -= get(c + S, ce, i - M);
        else if (i < 3 * M)     c[i] += get(c + S, ce, i - 2 * M) - get(c + S, ce, i - M);
        else                    c[i]  = get(c + S, ce, i - 2 * M);
    }

    return c + N;
}

template<typename T, int SZ = 8>
vector<T> karatsuba(const vector<T>& _a, const vector<T>& _b) {
    auto& a = sz(_a) > sz(_b) ? _a : _b, b = sz(_a) > sz(_b) ? _b : _a;
    if (sz(a) < SZ) return naive_product(a, b);
    vector<T> c(4 * (sz(a) + 1));
    karatsuba<T, SZ>(c.data(), a.data(), a.data() + sz(a), b.data(), b.data() + sz(b));
    c.erase(c.begin() + sz(a) + sz(b) - 1, c.end());
    return c;
}

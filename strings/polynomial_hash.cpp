// {{{ numeric/modnum.cpp }}}

#include <vector>
#include <chrono>
#include <random>
#include <array>
#include <cassert>
#include <iostream>

template<typename x_t, int EvaluationPoints, int AlphabetSize>
struct polynomial_hash {
    static x_t pow(int point, int exponent) {
        static std::vector<x_t> x(EvaluationPoints, 1);

        if (int(x.size()) == EvaluationPoints) {
            x.push_back(AlphabetSize);

            std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());

            for (int i = 1; i < EvaluationPoints; i++)
                x.push_back(AlphabetSize + rng() % std::max(256, AlphabetSize));
        }

        while (exponent * EvaluationPoints + point >= int(x.size()))
            for (int i = 0; i < EvaluationPoints; i++)
                x.push_back(*(x.end() - EvaluationPoints) * x[EvaluationPoints + i]);

        return x[exponent * EvaluationPoints + point];
    }

    int N;
    std::array<x_t, EvaluationPoints> data;

    polynomial_hash () : N(0) { data.fill(0); }

    polynomial_hash (x_t v) : N(1) { assert(v.v < AlphabetSize); data.fill(v); }

    polynomial_hash (int _N, std::array<x_t, EvaluationPoints> _data) : N(_N), data(_data) {}

    static polynomial_hash concatenate(polynomial_hash a, polynomial_hash b) {
        std::array<x_t, EvaluationPoints> res;
        for (int i = 0; i < EvaluationPoints; i++)
            res[i] = a.data[i] * pow(i, b.N) + b.data[i];
        return { a.N + b.N, res };
    }

    bool operator == (const polynomial_hash &o) const {
        return N == o.N && data == o.data;
    }

    bool operator < (const polynomial_hash &o) const {
        return N != o.N ? N < o.N : data < o.data;
    }

    friend std::ostream& operator << (std::ostream& o, const polynomial_hash& h) {
        o << h.N;
        for (int i = 0; i < EvaluationPoints; i++)
            o << " " << h.data[i];
        return o;
    }

    template<typename InputIterator>
    static std::vector<polynomial_hash> get_prefixes(InputIterator begin, InputIterator end) {
        std::vector<polynomial_hash> res = { polynomial_hash{} };
        for (InputIterator iter = begin; iter != end; iter = next(iter))
            res.push_back(concatenate(res.back(), polynomial_hash(x_t(*iter))));
        return res;
    }

    static polynomial_hash get_substring(const std::vector<polynomial_hash> &prefixes, int first, int last) {
        std::array<x_t, EvaluationPoints> res;
        for (int i = 0; i < EvaluationPoints; i++)
            res[i] = prefixes[last].data[i] - pow(i, last - first) * prefixes[first].data[i];
        return { last - first, res };
    }
};

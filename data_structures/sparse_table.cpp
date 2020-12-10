#include <vector>
#include <cassert>

template<typename T, typename IdempotentBinaryOperation = const T&(*)(const T&, const T&)>
struct sparse_table {
    int SZ;
    IdempotentBinaryOperation TT;
    std::vector<std::vector<T>> data;

    sparse_table() {}

    sparse_table(IdempotentBinaryOperation _TT) : TT(_TT) {}

    template<typename Function>
    sparse_table(int _SZ, IdempotentBinaryOperation _TT, Function fn) : TT(_TT) {
        construct(_SZ, fn);
    }

    template<typename Function>
    void construct(int _SZ, Function fn) {
        SZ = _SZ;

        const int L = 32 - __builtin_clz(std::max(SZ - 1, 1));
        data.assign(L, std::vector<T>(SZ));

        for (int i = 0; i < SZ; i++)
            data[0][i] = fn(i);

        for (int l = 0; l + 1 < L; l++)
            for (int i = 0; i + (2 << l) <= SZ; i++)
                data[l + 1][i] = TT(data[l][i], data[l][i + (1 << l)]);
    }

    // Accumulates the elements at indices in [i, j) in O(1)
    T accumulate(int first, int last) const {
        assert(0 <= first && first < last && last <= SZ);
        int l = last - first > 1 ? 31 - __builtin_clz(last - first - 1) : 0;
        return TT(data[l][first], data[l][last - (1 << l)]);
    }
};

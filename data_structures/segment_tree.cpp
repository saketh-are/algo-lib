#include <vector>
#include <cassert>

template<typename T, typename AssociativeOperation>
struct segment_tree {
    int SZ;
    T identity;
    AssociativeOperation TT;
    std::vector<T> data;

    segment_tree() {}

    segment_tree(int _SZ, T _identity, AssociativeOperation _TT)
            : SZ(_SZ), identity(_identity), TT(_TT) {
        data.resize(2 * SZ, identity);
    }

    // Returns the value at index i
    const T& operator[](int i) const {
        assert(0 <= i && i < SZ);
        return data[SZ + i];
    }

    // Assigns fn(i) at index i for each i in [0, SZ)
    template<typename Function>
    void assign(Function fn) {
        for (int i = 0; i < SZ; i++)
            data[SZ + i] = fn(i);
        for (int i = SZ - 1; i; i--)
            data[i] = TT(data[2 * i], data[2 * i + 1]);
    }

    // Assigns v at index i
    void assign(int i, T v) {
        assert(0 <= i && i < SZ);
        data[i += SZ] = v;
        for (i /= 2; i; i /= 2)
            data[i] = TT(data[2 * i], data[2 * i + 1]);
    }

    // Returns the result of a left fold of the elements at indices in [first, last) over TT
    T accumulate(int first, int last) const {
        assert(0 <= first && last <= SZ);
        T left = identity, right = identity;
        for (first += SZ, last += SZ; first < last; first /= 2, last /= 2) {
            if (first & 1) left  = TT(left, data[first++]);
            if (last  & 1) right = TT(data[--last], right);
        }
        return TT(left, right);
    }
};

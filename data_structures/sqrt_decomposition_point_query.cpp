#include <vector>
#include <cassert>

template<typename T, typename CommutativeOperation>
struct sqrt_decomposition_point_query {
    int SZ;
    T t_identity;
    CommutativeOperation TT;

    int SQRT;
    std::vector<T> data;
    std::vector<T> block_data;

    sqrt_decomposition_point_query() {}

    sqrt_decomposition_point_query(int _SZ, T _t_identity, CommutativeOperation _TT)
            : SZ(_SZ), t_identity(_t_identity), TT(_TT) {
        SQRT = 0;
        while (SQRT * SQRT < SZ)
            SQRT++;

        data.assign(SZ, t_identity);
        block_data.assign(SQRT, t_identity);
    }

    template<typename Function>
    void assign(Function fn) {
        for (int i = 0; i < SZ; i++)
            data[i] = fn(i);
    }

public:
    // Replaces the current value at index i with TT(current value, v)
    void add(int i, T v) {
        data[i] = TT(data[i], v);
    }

    void add_to_range(int first, int last, T v) {
        if (last - first < SQRT) {
            for (int i = first; i < last; i++)
                data[i] = TT(data[i], v);
            return;
        }

        const int first_block = (first + SQRT - 1) / SQRT;
        const int last_block  = last / SQRT;

        for (int i = first; i < first_block * SQRT; i++)
            data[i] = TT(data[i], v);

        for (int block = first_block; block < last_block; block++)
            block_data[block] = TT(block_data[block], v);

        for (int i = last_block * SQRT; i < last; i++)
            data[i] = TT(data[i], v);
    }

    // Returns the current value at index i
    T read(int i) const {
        return TT(data[i], block_data[i / SQRT]);
    }
};

// {{{ data_structures/segment_tree.cpp }}}

#include <cassert>

template<typename T, typename AssociativeOperation>
struct searchable_segment_tree : segment_tree<T, AssociativeOperation> {
    using segment_tree<T, AssociativeOperation>::SZ;
    using segment_tree<T, AssociativeOperation>::identity;
    using segment_tree<T, AssociativeOperation>::TT;
    using segment_tree<T, AssociativeOperation>::data;

    searchable_segment_tree() {}

    /* Rounds up internal size to the next power of 2 to enable binary search.
     */
    searchable_segment_tree(int _SZ, T _identity, AssociativeOperation _TT) :
        segment_tree<T, AssociativeOperation>(1 << (32 - __builtin_clz(_SZ - 1)), _identity, _TT) {}

    /* Returns the smallest index "last" >= first such that p(accumulate(first, last))
     * returns true. Returns SZ + 1 if no such index exists. Requires that
     * p(accumulate(first, last)) is non-decreasing as last increases.
     */
    template<typename Predicate>
    int binary_search(int first, Predicate p) const {
        assert(0 <= first && first <= SZ);

        if (p(identity))
            return first;

        first += SZ;

        T accumulator = identity;

        auto try_extend = [&](int bit) {
            assert(__builtin_ctz(first) >= bit);

            if (first + (1 << bit) > 2 * SZ)
                return false;

            T extended = TT(accumulator, data[first >> bit]);

            if (p(extended))
                return false;

            accumulator = extended;
            first += 1 << bit;
            return true;
        };

        int bit = 0;

        while (!(first & (1 << bit)) || try_extend(bit))
            bit++;

        while (--bit >= 0)
            try_extend(bit);

        return first - SZ + 1;
    }
};

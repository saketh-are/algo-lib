// {{{ data_structures/segment_tree.cpp }}}
template<typename T, typename BinaryOperation>
struct searchable_segment_tree : segment_tree<T, BinaryOperation> {
    searchable_segment_tree() {}

    /* Rounds up internal size to the nearest power of 2 to enable binary search.
     */
    searchable_segment_tree(int SZ, T identity, BinaryOperation TT) :
        segment_tree<T, BinaryOperation>(1 << (32 - __builtin_clz(SZ - 1)), identity, TT) {}

    /* Returns the smallest len in [0, SZ] such that p(accumulate(0, len)) returns true.
     * Returns SZ + 1 if no such len exists.
     * Requires that p(accumulate(len)) is non-decreasing in len.
     */
    template<typename Predicate>
    int binary_search(Predicate p) const {
        if (!p(this->data[1])) return this->SZ + 1;

        int loc = 1;
        T accumulator = this->identity;

        if (p(accumulator)) return 0;

        while (loc < this->SZ) {
            T combined = this->TT(accumulator, this->data[2 * loc]);
            if (p(combined)) {
                loc = 2 * loc;
            } else {
                loc = 2 * loc + 1;
                accumulator = combined;
            }
        }

        return loc - this->SZ + 1;
    }
};

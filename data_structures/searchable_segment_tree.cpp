template<typename T, typename F> struct full_segment_tree : segment_tree<T, F> {
    // Internal size is rounded up so that all layers are full
    full_segment_tree() {}
    full_segment_tree(int _SZ, T _id, F _tt) :
        segment_tree<T, F>(1 << (32 - __builtin_clz(_SZ - 1)), _id, _tt) {}

    /*
     * Returns the first i in [0, S] such that comp(this(0, i))
     * evaluates to true. Returns -1 if no such i exists.
     * Requires that comp(this(0, i)) is non-decreasing in i.
     */
    template<typename C> int lower_bound(C comp) const {
        if (!comp(this->table[1])) return -1;
        if (comp(T{})) return 0;

        int loc = 1;
        for (T cur{}; loc < this->SZ; ) {
            T mid = this->tt(cur, this->table[2 * loc]);
            if (comp(mid)) {
                loc = 2 * loc;
            } else {
                cur = mid;
                loc = 2 * loc + 1;
            }
        }
        return loc - this->SZ + 1;
    }
};

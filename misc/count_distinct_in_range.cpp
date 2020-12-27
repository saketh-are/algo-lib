// {{{ data_structures/segment_tree_persistent.cpp }}}

#include <functional>
#include <vector>
#include <algorithm>
#include <cassert>

template<typename T>
struct count_distinct_in_range {
    segment_tree_persistent<int, std::plus<int>> st;

    template<typename InputIterator>
    count_distinct_in_range(InputIterator first, InputIterator last, int copies_allowed = 1) {
        assert(copies_allowed >= 1);

        std::vector<std::remove_reference_t<decltype(*first)>> universe(first, last);
        std::sort(universe.begin(), universe.end());
        universe.erase(std::unique(universe.begin(), universe.end()), universe.end());

        std::vector<std::vector<int>> occurrences(universe.size());

        int SZ = 0;
        for (InputIterator iter = first; iter != last; iter++) {
            int val = int(lower_bound(universe.begin(), universe.end(), *iter) - universe.begin());
            occurrences[val].push_back(SZ);
            SZ++;
        }

        st = decltype(st)(SZ, 0, std::plus<int>());

        std::vector<int> successor(SZ, -1);

        for (int i = 0; i < universe.size(); i++) {
            for (int j = 0; j < std::min(copies_allowed, int(occurrences[i].size())); j++)
                st.assign(occurrences[i][j], 1, -1);

            for (int j = 0; j + copies_allowed < int(occurrences[i].size()); j++)
                successor[occurrences[i][j]] = occurrences[i][j + copies_allowed];
        }

        for (int i = 0; i < SZ; i++)
            if (successor[i] != -1)
                st.assign(successor[i], 1, i);
    }

    /* Returns the number of elements at indices in [first, last) counting only
     * the first 'copies_allowed' appearances of each distinct value.
     */
    int get_count(int first, int last) {
        return st.accumulate(first, last, first);
    }
};

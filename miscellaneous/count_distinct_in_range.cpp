template<typename T>
struct count_distinct_in_range {
    segment_tree_persistent<int, decltype(plus<int>())> st;

    count_distinct_in_range(const vector<T> &vals, int copies_allowed = 1) {
        assert(copies_allowed >= 1);

        vector<T> uniq = vals;
        sort(uniq.begin(), uniq.end());
        uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());

        vector<vector<int>> occur(uniq.size());
        for (int i = 0; i < vals.size(); i++) {
            int v = lower_bound(uniq.begin(), uniq.end(), vals[i]) - uniq.begin();
            occur[v].push_back(i);
        }

        st = segment_tree_persistent(vals.size(), 0, plus<int>());

        vector<int> successor(vals.size(), -1);
        for (int i = 0; i < occur.size(); i++) {
            for (int j = 0; j < min(copies_allowed, int(occur[i].size())); j++)
                st.assign(occur[i][j], 1, -1);

            for (int j = 0; j + copies_allowed < occur[i].size(); j++)
                successor[occur[i][j]] = occur[i][j + copies_allowed];
        }

        for (int i = 0; i < vals.size(); i++)
            if (successor[i] != -1)
                st.assign(successor[i], 1, i);
    }

    /* Returns the number of distinct elements appearing at indices in [L, R) of 'vals'.
     * The first 'copies_allowed' copies of each distinct value are counted.
     */
    int get_count(int L, int R) {
        return st.accumulate(L, R, L);
    }
};

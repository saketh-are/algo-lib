struct knuth_morris_pratt {
    int SZ;
    vector<int> pattern;
    vector<int> suffix_link;

    size_t append(int matched, int c) const {
        while (matched > 0 && pattern[matched] != c)
            matched = suffix_link[matched];
        return matched + int(pattern[matched] == c);
    }

    knuth_morris_pratt() : SZ(0) { }

    template<typename I>
    knuth_morris_pratt(I begin, I end) { initialize(begin, end); }

    template<typename I>
    void initialize(I begin, I end) {
        pattern.resize(end - begin);
        copy(begin, end, pattern.begin());
        SZ = int(pattern.size());
        suffix_link.resize(SZ + 1);
        for (int matched = 1; matched < SZ; matched++)
            suffix_link[matched + 1] = append(suffix_link[matched], pattern[matched]);
    }

    template<typename I, typename F>
    void find_matches(I begin, I end, F consume) const {
        int i = 0;
        int matched = 0;
        for (I iter = begin; iter != end; iter++, i++) {
            matched = append(matched, *iter);
            if (matched == SZ) {
                consume(i - SZ + 1);
                matched = suffix_link[matched];
            }
        }
    }

    template<typename I>
    int count_matches(I begin, I end) const {
        int count = 0;
        find_matches(begin, end, [&](int i) { count++; });
        return count;
    }

    template<typename I>
    vector<int> indices_of_matches(I begin, I end) const {
        vector<int> indices;
        find_matches(begin, end, [&](int i) { indices.push_back(i); });
        return indices;
    }
};

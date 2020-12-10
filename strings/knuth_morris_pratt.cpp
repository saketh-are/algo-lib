#include <vector>

struct knuth_morris_pratt {
    int SZ;
    std::vector<int> pattern;
    std::vector<int> suffix_link;

    int append(int matched, int c) const {
        while (matched > 0 && pattern[matched] != c)
            matched = suffix_link[matched];
        return matched + int(pattern[matched] == c);
    }

    knuth_morris_pratt() : SZ(0) { }

    template<typename InputIterator>
    knuth_morris_pratt(InputIterator begin, InputIterator end) {
        initialize(begin, end);
    }

    template<typename InputIterator>
    void initialize(InputIterator begin, InputIterator end) {
        pattern.resize(end - begin);
        copy(begin, end, pattern.begin());

        SZ = int(pattern.size());
        suffix_link.resize(SZ + 1);

        for (int matched = 1; matched < SZ; matched++)
            suffix_link[matched + 1] = append(suffix_link[matched], pattern[matched]);
    }

    template<typename InputIterator, typename F>
    void find_matches(InputIterator begin, InputIterator end, F consume) const {
        int i = 0;
        int matched = 0;
        for (InputIterator iter = begin; iter != end; iter++, i++) {
            matched = append(matched, *iter);
            if (matched == SZ) {
                consume(i - SZ + 1);
                matched = suffix_link[matched];
            }
        }
    }

    template<typename InputIterator>
    int count_matches(InputIterator begin, InputIterator end) const {
        int count = 0;
        find_matches(begin, end, [&](__attribute((unused))int index) { count++; });
        return count;
    }

    template<typename InputIterator>
    std::vector<int> indices_of_matches(InputIterator begin, InputIterator end) const {
        std::vector<int> indices;
        find_matches(begin, end, [&](int index) { indices.push_back(index); });
        return indices;
    }
};

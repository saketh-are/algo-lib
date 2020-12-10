#include <vector>
#include <bitset>

#pragma GCC optimize("unroll-loops")
#pragma GCC target("popcnt,tune=native")

template<int MIN_CHAR, int SIGMA, int MAX_LEN>
struct mutable_string_bitset {
    std::vector<std::bitset<MAX_LEN>> occur;

    template<typename InputIterator>
    mutable_string_bitset(InputIterator first, InputIterator last) : occur(SIGMA) {
        int i = 0;
        for (InputIterator iter = first; iter != last; iter++)
            occur[*iter - MIN_CHAR][i++] = true;
    }

    void assign(int i, int c) {
        for (int a = 0; a < SIGMA; a++)
            occur[a][i] = false;
        occur[c - MIN_CHAR][i] = true;
    }

    /* Counts occurrences of the pattern [first, last) within the substring [L, R)
     * O(|last - first| * MAX_LEN / MACHINE_WORD_SIZE)
     */
    template<typename InputIterator>
    int count_matches(InputIterator first, InputIterator last, int L = 0, int R = MAX_LEN) const {
        static std::bitset<MAX_LEN> match;
        match.set(); // sets all bits to true

        int i = 0;
        for (InputIterator iter = first; iter != last; iter++)
            match &= occur[*iter - MIN_CHAR] >> (i++);

        int count = 0;

        int min_pos = L, max_pos = R - i + 1;

        while (min_pos < max_pos && min_pos % 64)
            count += match[min_pos++];

        while (min_pos < max_pos && max_pos % 64)
            count += match[--max_pos];

        static uint64_t *data = (uint64_t*)&match;
        for (int inx = min_pos / 64; inx < max_pos / 64; inx++)
            count += __builtin_popcountll(data[inx]);

        return count;
    }
};

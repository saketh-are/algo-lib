// {{{ strings/suffix_automaton.cpp }}}
// {{{ strings/knuth_morris_pratt.cpp }}}

#include <vector>
#include <cmath>

template<int MIN_CHAR, int SIGMA>
struct mutable_string {
    int SZ;
    std::vector<int> data;

    int BLOCK_SZ;
    std::vector<suffix_automaton<MIN_CHAR, SIGMA>> blocks;

    template<typename InputIterator>
    mutable_string(InputIterator begin, InputIterator end) : data(begin, end) {
        SZ = int(data.size());
        BLOCK_SZ = int(ceil(std::sqrt(data.size())));
        for (int i = 0; i < SZ; i += BLOCK_SZ)
            blocks.emplace_back(data.begin() + i, data.begin() + std::min(i + BLOCK_SZ, SZ));
    }

    std::vector<int>::iterator block_start(int block_id) {
        return data.begin() + std::min(block_id * BLOCK_SZ, SZ);
    }
    std::vector<int>::const_iterator block_start(int block_id) const {
        return data.begin() + std::min(block_id * BLOCK_SZ, SZ);
    }

    // O(sqrt(SZ))
    void assign(int i, int c) {
        data[i] = c;
        int block_id = i / BLOCK_SZ;
        blocks[block_id].initialize(block_start(block_id), block_start(block_id + 1));
    }

    // O(min(|end - begin| * sqrt(SZ), |end - begin| + len))
    template<typename InputIterator>
    int count_matches_in_substring(InputIterator begin, InputIterator end, int L, int R) const {
        static knuth_morris_pratt kmp;
        kmp.initialize(begin, end);

        int first_block_id = (L + BLOCK_SZ - 1) / BLOCK_SZ;
        int last_block_id  = R / BLOCK_SZ;

        int expected_work =
            first_block_id * BLOCK_SZ - L
            + (last_block_id - first_block_id) * 3 * kmp.SZ
            + R - last_block_id * BLOCK_SZ;

        if (first_block_id >= last_block_id || kmp.SZ > BLOCK_SZ || expected_work >= R - L)
            return kmp.count_matches(data.begin() + L, data.begin() + R);

        int count = 0;

        count += kmp.count_matches(data.begin() + L, block_start(first_block_id) + kmp.SZ - 1);

        for (int block_id = first_block_id; block_id < last_block_id; block_id++) {
            count += blocks[block_id].count_occurrences(begin, end);

            if (block_id != first_block_id) {
                auto boundary = block_start(block_id);
                count += kmp.count_matches(boundary - kmp.SZ + 1, boundary + kmp.SZ - 1);
            }
        }

        count += kmp.count_matches(block_start(last_block_id) - kmp.SZ + 1, data.begin() + R);

        return count;
    }

    template<typename InputIterator>
    int count_matches(InputIterator begin, InputIterator end) const {
        return count_matches_in_substring(begin, end, 0, SZ);
    }
};

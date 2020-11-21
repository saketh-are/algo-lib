template<int MIN_CHAR, int SIGMA>
struct mutable_string {
    int SZ;
    vector<int> data;

    int BLOCK_SZ;
    vector<suffix_automaton<MIN_CHAR, SIGMA>> blocks;

    template<typename I>
    mutable_string(I begin, I end) : data(begin, end) {
        SZ = data.size();
        BLOCK_SZ = ceil(sqrt(data.size()));
        for (int i = 0; i < SZ; i += BLOCK_SZ)
            blocks.emplace_back(data.begin() + i, data.begin() + min(i + BLOCK_SZ, SZ));
    }

    vector<int>::iterator block_start(int block_id) {
        return data.begin() + min(block_id * BLOCK_SZ, SZ);
    }
    vector<int>::const_iterator block_start(int block_id) const {
        return data.begin() + min(block_id * BLOCK_SZ, SZ);
    }

    // O(sqrt(SZ))
    void assign(int i, int c) {
        data[i] = c;
        int block_id = i / BLOCK_SZ;
        blocks[block_id].initialize(block_start(block_id), block_start(block_id + 1));
    }

    // O(min(|end - begin| * sqrt(SZ), |end - begin| + len))
    template<typename I>
    int count_matches_in_substring(I begin, I end, int pos, int len) const {
        static knuth_morris_pratt kmp;
        kmp.initialize(begin, end);

        int first_block_id = (pos + BLOCK_SZ - 1) / BLOCK_SZ;
        int last_block_id  = (pos + len) / BLOCK_SZ;

        if (kmp.SZ * 3 >= BLOCK_SZ || first_block_id >= last_block_id)
            return kmp.count_matches(data.begin() + pos, data.begin() + pos + len);

        int count = 0;

        count += kmp.count_matches(data.begin() + pos, block_start(first_block_id) + kmp.SZ - 1);

        for (int block_id = first_block_id; block_id < last_block_id; block_id++) {
            count += blocks[block_id].count_occurences(begin, end);

            if (block_id != first_block_id) {
                auto boundary = block_start(block_id);
                count += kmp.count_matches(boundary - kmp.SZ + 1, boundary + kmp.SZ - 1);
            }
        }

        count += kmp.count_matches(block_start(last_block_id) - kmp.SZ + 1, data.begin() + pos + len);

        return count;
    }

    template<typename I>
    int count_matches(I begin, I end) const {
        return count_matches_in_substring(begin, end, 0, SZ);
    }
};

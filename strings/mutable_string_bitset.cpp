#pragma GCC optimize("unroll-loops")
#pragma GCC target("popcnt,tune=native")
template<int MIN_CHAR, int SIGMA, int MAX_LEN>
struct mutable_string_bitset {
    vector<bitset<MAX_LEN>> occur;

    template<typename I>
    mutable_string_bitset(I begin, I end) : occur(SIGMA) {
        int i = 0;
        for (I iter = begin; iter != end; iter++)
            occur[*iter - MIN_CHAR][i++] = 1;
    }

    void assign(int i, int c) {
        for (int a = 0; a < SIGMA; a++)
            occur[a][i] = a == (c - MIN_CHAR);
    }

    // O(|end - begin| * MAX_LEN / MACHINE_WORD_SIZE)
    template<typename I>
    int count_matches(I begin, I end, int pos = 0, int len = MAX_LEN) const {
        static bitset<MAX_LEN> match;
        static uint64_t* data = (uint64_t*)&match;

        match.set();
        int i = 0;
        for (I iter = begin; iter != end; iter++)
            match &= occur[*iter - MIN_CHAR] >> (i++);

        int count = 0;

        int min_pos = pos, max_pos = pos + len - i + 1;
        while (min_pos < max_pos && min_pos % 64)
            count += match[min_pos++];
        while (min_pos < max_pos && max_pos % 64)
            count += match[--max_pos];
        for (int inx = min_pos / 64; inx < max_pos / 64; inx++)
            count += __builtin_popcountll(data[inx]);

        return count;
    }
};

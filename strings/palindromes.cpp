struct palindromes {
    int SZ;
    vector<int> pal_len;

    template<typename I>
    palindromes(I begin, I end) {
        SZ = int(end - begin);
        pal_len.resize(max(2 * SZ - 1, 0));

        auto match = [&](int c, int r) {
            if ((c - r) < -1 || (c + r) > int(pal_len.size())) return false;
            return (c - r)&1 || *(begin + (c - r)/2) == *(begin + (c + r)/2);
        };

        for (int cen = 0, lim = -1, ref; cen < int(pal_len.size()); cen++) {
            if (lim > cen)
                pal_len[cen] = min(pal_len[2 * ref - cen], lim - cen);
            while (match(cen, pal_len[cen] + 1))
                pal_len[cen]++;
            if (cen + pal_len[cen] > lim) {
                lim = cen + pal_len[cen];
                ref = cen;
            }
        }
    }

    int longest_centered_at(int index) const {
        assert(0 <= index && index < SZ);
        return pal_len[2 * index];
    }

    int longest_centered_right_of(int index) const {
        assert(0 <= index && index < SZ - 1);
        return pal_len[2 * index + 1];
    }

    static struct greater {
        int operator()(int x, int y) const { return max(x, y); }
    } len_cmp;
    mutable sparse_table<int, greater> pal_len_rmq;

    int longest_within_substring(int pos, int len) const {
        assert(0 <= pos && pos + len <= SZ);
        if (!pal_len_rmq.SZ)
            pal_len_rmq = sparse_table<int, greater>(int(pal_len.size()), len_cmp,
                    [&](int i) { return pal_len[i]; });

        int lo = 0, hi = len + 1;
        while (hi - lo > 1) {
            int mi = lo + (hi - lo) / 2;
            int min_c = min(2 * (pos + mi / 2), 2 * (pos + (mi - 2 + 1) / 2) + 1);
            int max_c = max(2 * (pos + len - mi / 2) - 1, 2 * (pos + len - (mi + 1) / 2));
            if (min_c < max_c && pal_len_rmq(min_c, max_c) >= mi)
                lo = mi;
            else
                hi = mi;
        }
        return lo;
    }
};

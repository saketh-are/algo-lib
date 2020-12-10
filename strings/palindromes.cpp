// {{{ data_structures/sparse_table.cpp }}}

#include <vector>
#include <cassert>

struct palindromes {
    int SZ;
    std::vector<int> width;

    template<typename I>
    palindromes(I begin, I end) {
        SZ = int(end - begin);
        width.resize(std::max(2 * SZ - 1, 0));

        auto match = [&](int c, int r) {
            if ((c - r) < -1 || (c + r) > int(width.size())) return false;
            return (c - r)&1 || *(begin + (c - r)/2) == *(begin + (c + r)/2);
        };

        for (int cen = 0, lim = -1, ref; cen < int(width.size()); cen++) {
            if (lim > cen)
                width[cen] = std::min(width[2 * ref - cen], lim - cen);
            while (match(cen, width[cen] + 1))
                width[cen]++;
            if (cen + width[cen] > lim) {
                lim = cen + width[cen];
                ref = cen;
            }
        }
    }

    int longest_centered_at(int index) const {
        assert(0 <= index && index < SZ);
        return width[2 * index];
    }

    int longest_centered_right_of(int index) const {
        assert(0 <= index && index < SZ - 1);
        return width[2 * index + 1];
    }

    int longest_within_substring(int pos, int len) const {
        static sparse_table<int>
            width_rmq(int(width.size()), std::max<int>, [&](int i) { return width[i]; });

        assert(0 <= pos && pos + len <= SZ);

        int lo = 0, hi = len + 1;
        while (hi - lo > 1) {
            int mi = lo + (hi - lo) / 2;
            int min_c = std::min(2 * (pos + mi / 2), 2 * (pos + (mi - 2 + 1) / 2) + 1);
            int max_c = std::max(2 * (pos + len - mi / 2) - 1, 2 * (pos + len - (mi + 1) / 2));
            if (min_c < max_c && width_rmq.accumulate(min_c, max_c) >= mi)
                lo = mi;
            else
                hi = mi;
        }
        return lo;
    }
};

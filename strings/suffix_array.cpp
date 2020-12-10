// {{{ data_structures/sparse_table.cpp }}}

#include <vector>
#include <algorithm>

enum BACKFILL { CYCLIC, NEG_INF, POS_INF };
template<BACKFILL FILL>
struct suffix_array {
    int SZ;
    std::vector<int> input;
    std::vector<int> rank_of;
    std::vector<int> lcp_between_ranks;
    std::vector<int> first_suffix_with_rank;

    int read_rank(const std::vector<int>& v, int i) const {
        if (i < SZ) {
            return v[i];
        } else switch (FILL) {
            case CYCLIC:  return v[i - SZ];
            case NEG_INF: return -1;
            case POS_INF: return SZ;
        }
    }

    int rank_of_char  (int i) const { return read_rank(input, i); }
    int rank_of_suffix(int i) const { return read_rank(rank_of, i); }
    int suffix_at_rank(int r) const { return first_suffix_with_rank[r]; }

    template<typename InputIterator>
    suffix_array(InputIterator begin, InputIterator end) {
        std::vector<int> uniq(begin, end);
        std::sort(uniq.begin(), uniq.end());
        uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());

        for (InputIterator iter = begin; iter != end; iter = next(iter))
            input.push_back(int((lower_bound(uniq.begin(), uniq.end(), *iter) - uniq.begin())));

        SZ = int(input.size());

        rank_of = input;

        std::vector<int> ct(SZ);
        for (int i = 0; i < SZ; i++) ct[input[i]]++;
        for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

        std::vector<int> by_rank(SZ);
        for (int i = SZ - 1; i >= 0; i--) by_rank[--ct[input[i]]] = i;

        std::vector<int> new_ranks(SZ);
        std::vector<int> by_rank_updated(SZ);
        for (int len = 1; len < SZ; len *= 2) {
            int tail = 0;
            if (FILL == NEG_INF)
                for (int i = SZ - len; i < SZ; i++)
                    by_rank_updated[tail++] = i;
            for (int rank = 0; rank < SZ; rank++)
                if (by_rank[rank] >= len)
                    by_rank_updated[tail++] = by_rank[rank] - len;
                else if (FILL == CYCLIC)
                    by_rank_updated[tail++] = by_rank[rank] - len + SZ;
            if (FILL == POS_INF)
                for (int i = SZ - len; i < SZ; i++)
                    by_rank_updated[tail++] = i;
            swap(by_rank, by_rank_updated);

            fill(ct.begin(), ct.end(), 0);
            for (int i = 0; i < SZ; i++) ct[rank_of[i]]++;
            for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

            for (int i = SZ - 1; i >= 0; i--)
                by_rank_updated[--ct[rank_of[by_rank[i]]]] = by_rank[i];
            swap(by_rank, by_rank_updated);

            std::pair<int, int> prev = { rank_of_suffix(by_rank[0]), rank_of_suffix(by_rank[0] + len) };
            for (int i = 1; i < SZ; i++) {
                std::pair<int, int> cur = { rank_of_suffix(by_rank[i]), rank_of_suffix(by_rank[i] + len) };
                new_ranks[by_rank[i]] = new_ranks[by_rank[i - 1]] + (prev != cur);
                prev = cur;
            }
            swap(rank_of, new_ranks);
        }

        int R = *max_element(rank_of.begin(), rank_of.end()) + 1;

        first_suffix_with_rank.resize(R);
        for (int i = SZ - 1; i >= 0; i--)
            first_suffix_with_rank[rank_of_suffix(i)] = i;

        lcp_between_ranks.resize(SZ - 1);
        for (int i = 0, len = 0; i < SZ; i++) {
            if (rank_of_suffix(i) == R - 1)
                continue;
            int j = suffix_at_rank(rank_of_suffix(i) + 1);
            while (len < SZ && rank_of_char(i + len) == rank_of_char(j + len))
                len++;
            lcp_between_ranks[rank_of_suffix(i)] = len;
            if (len) --len;
        }
    }

    int longest_common_prefix(int i, int j) const {
        static sparse_table<int>
            lcp_between_ranks_rmq(SZ - 1, std::min<int>, [&](int r) { return lcp_between_ranks[r]; });

        assert(FILL != CYCLIC && 0 <= i && i < SZ && 0 <= j && j < SZ);
        int ri = rank_of_suffix(i);
        int rj = rank_of_suffix(j);
        if (ri == rj) return FILL == CYCLIC ? SZ : SZ - i;
        if (ri  > rj) std::swap(ri, rj);
        return lcp_between_ranks_rmq.accumulate(ri, rj);
    }

    std::pair<int, int> ranks_with_prefix(int pos, int len) const {
        assert(0 <= pos && pos + len <= SZ);
        const std::vector<int> &F = first_suffix_with_rank;
        auto less_than = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) < arg; };
        auto geq       = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) >= arg; };
        return std::make_pair(
            lower_bound(F.begin(), F.begin() + rank_of_suffix(pos), len, less_than) - F.begin(),
            lower_bound(F.begin() + rank_of_suffix(pos), F.end(), len, geq) - F.begin()
        );
    }

    int first_occurrence_of(int pos, int len) const {
        static sparse_table<int>
            by_rank_rmq(SZ, std::min<int>, [&](int r) { return first_suffix_with_rank[r]; });

        std::pair<int, int> ranks_of_occurrences = ranks_with_prefix(pos, len);
        return by_rank_rmq.accumulate(ranks_of_occurrences.first, ranks_of_occurrences.second);
    }
};

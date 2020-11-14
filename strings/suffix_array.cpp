enum BACKFILL { CYCLIC, NEG_INF, POS_INF };
template<BACKFILL FILL>
struct suffix_array {
    int SZ;
    vector<vector<int>> rank_of;
    vector<int> by_rank;
    vector<int> lcp_between_ranks;

    template<typename I>
    suffix_array(I begin, I end) {
        vector<int> input;
        for (I iter = begin; iter != end; iter = next(iter))
            input.push_back(int(*iter));

        SZ = int(input.size());

        vector<int> uniq = input;
        sort(uniq.begin(), uniq.end());
        uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());
        for (int &val : input)
            val = int((lower_bound(uniq.begin(), uniq.end(), val) - uniq.begin()));

        rank_of.push_back(input);

        vector<int> ct(SZ);
        for (int i = 0; i < SZ; i++) ct[input[i]]++;
        for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

        by_rank.resize(SZ);
        for (int i = SZ - 1; i >= 0; i--) by_rank[--ct[input[i]]] = i;

        vector<int> by_rank_updated(SZ);
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
            for (int i = 0; i < SZ; i++) ct[rank_of.back()[i]]++;
            for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

            for (int i = SZ - 1; i >= 0; i--)
                by_rank_updated[--ct[rank_of.back()[by_rank[i]]]] = by_rank[i];
            swap(by_rank, by_rank_updated);

            vector<int> new_ranks(SZ);
            pair<int, int> prev = { rank_of_suffix(by_rank[0]), rank_of_suffix(by_rank[0] + len) };
            for (int i = 1; i < SZ; i++) {
                pair<int, int> cur = { rank_of_suffix(by_rank[i]), rank_of_suffix(by_rank[i] + len) };
                new_ranks[by_rank[i]] = new_ranks[by_rank[i - 1]] + (prev != cur);
                prev = cur;
            }
            rank_of.push_back(new_ranks);
        }

        lcp_between_ranks.resize(SZ - 1);
        for (int i = 0, len = 0; i < SZ; i++) {
            if (rank_of_suffix(i) == SZ - 1)
                continue;
            int j = suffix_at_rank(rank_of_suffix(i) + 1);
            while (i + len < SZ && j + len < SZ && input[i + len] == input[j + len])
                len++;
            lcp_between_ranks[rank_of_suffix(i)] = len;
            if (len) --len;
        }
    }

    int rank_of_suffix(int i) const {
        if (i < SZ) {
            return rank_of.back()[i];
        } else switch (FILL) {
            case CYCLIC:  return rank_of.back()[i - SZ];
            case NEG_INF: return -1;
            case POS_INF: return SZ;
        }
    }

    int suffix_at_rank(int r) const {
        return by_rank[r];
    }

    struct smaller {
        int operator()(int x, int y) const { return min(x, y); }
    } cmp;

    int longest_common_prefix(int i, int j) const {
        static sparse_table<int, smaller>lcp_between_ranks_rmq(SZ - 1, cmp,
                [&](int r) { return lcp_between_ranks[r]; });
        assert(FILL != CYCLIC && 0 <= i && i < SZ && 0 <= j && j < SZ);
        if (i == j) return SZ - i;
        int ri = rank_of_suffix(i);
        int rj = rank_of_suffix(j);
        if (ri > rj) swap(ri, rj);
        return lcp_between_ranks_rmq(ri, rj);
    }

    pair<int, int> ranks_with_prefix(int pos, int len) const {
        assert(0 <= pos && pos + len <= SZ);
        auto less_than = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) < arg; };
        auto geq       = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) >= arg; };
        return make_pair(
            lower_bound(by_rank.begin(), by_rank.begin() + rank_of_suffix(pos), len, less_than) - by_rank.begin(),
            lower_bound(by_rank.begin() + rank_of_suffix(pos), by_rank.end(), len, geq) - by_rank.begin()
        );
    }

    int first_occurence_of(int pos, int len) const {
        static sparse_table<int, smaller> by_rank_rmq(SZ, cmp, [&](int r) { return by_rank[r]; });
        pair<int, int> ranks_of_occurences = ranks_with_prefix(pos, len);
        return by_rank_rmq(ranks_of_occurences.first, ranks_of_occurences.second);
    }
};

enum BACKFILL { CYCLIC, NEG_INF, POS_INF };
template<BACKFILL FILL>
struct suffix_array {
    int SZ;
    vector<vector<int>> rank_of;
    vector<int> by_rank;

    int rank(int i) const {
        if (i < SZ) {
            return rank_of.back()[i];
        } else switch (FILL) {
            case CYCLIC:  return rank_of.back()[i - SZ];
            case NEG_INF: return -1;
            case POS_INF: return SZ;
        }
    }

    template<typename I>
    suffix_array(I begin, I end) {
        vi input;
        for (I iter = begin; iter != end; iter = next(iter))
            input.push_back(*iter);

        SZ = int(input.size());

        vi uniq = input;
        sort(uniq.begin(), uniq.end());
        uniq.erase(unique(uniq.begin(), uniq.end()), uniq.end());
        for (int &val : input)
            val = (lower_bound(uniq.begin(), uniq.end(), val) - uniq.begin());

        rank_of.push_back(input);

        vi ct(SZ);
        for (int i = 0; i < SZ; i++) ct[input[i]]++;
        for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

        by_rank.resize(SZ);
        for (int i = SZ - 1; i >= 0; i--) by_rank[--ct[input[i]]] = i;

        vector<int> by_rank_updated(SZ);
        for (int len = 1; len < SZ; len *= 2) {
            int tail = 0;
            if (FILL == NEG_INF)
                for (int i = SZ - len; i < SZ; i++)
                    by_rank[tail++] = i;
            for (int rank = 0; rank < SZ; rank++)
                if (by_rank[rank] >= len)
                    by_rank[tail++] = by_rank[rank] - len;
                else if (FILL == CYCLIC)
                    by_rank[tail++] = by_rank[rank] - len + SZ;
            if (FILL == POS_INF)
                for (int i = SZ - len; i < SZ; i++)
                    by_rank[tail++] = i;

            fill(ct.begin(), ct.end(), 0);
            for (int i = 0; i < SZ; i++) ct[rank_of.back()[i]]++;
            for (int i = 0; i < SZ - 1; i++) ct[i + 1] += ct[i];

            for (int i = SZ - 1; i >= 0; i--)
                by_rank_updated[--ct[rank_of.back()[by_rank[i]]]] = by_rank[i];
            swap(by_rank, by_rank_updated);

            vi new_ranks(SZ);
            pair<int, int> prev = { rank(by_rank[0]), rank(by_rank[0] + len) };
            for (int i = 1; i < SZ; i++) {
                pair<int, int> cur = { rank(by_rank[i]), rank(by_rank[i] + len) };
                new_ranks[by_rank[i]] = new_ranks[by_rank[i - 1]] + (prev != cur);
                prev = cur;
            }
            rank_of.push_back(new_ranks);
        }
    }
};

enum BACKFILL { CYCLIC, NEG_INF, POS_INF };
template<BACKFILL FILL>
struct suffix_array {
    int SZ;
    vector<vector<int>> rank_of;
    vector<int> by_rank;

    vector<int> lcp;
    static struct smaller {
        int operator()(int x, int y) const { return min(x, y); }
    } lcp_cmp;
    sparse_table<int, smaller> lcp_rmq;

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

            vi new_ranks(SZ);
            pair<int, int> prev = { rank(by_rank[0]), rank(by_rank[0] + len) };
            for (int i = 1; i < SZ; i++) {
                pair<int, int> cur = { rank(by_rank[i]), rank(by_rank[i] + len) };
                new_ranks[by_rank[i]] = new_ranks[by_rank[i - 1]] + (prev != cur);
                prev = cur;
            }
            rank_of.push_back(new_ranks);
        }

        lcp.resize(SZ - 1);
        for (int i = 0, len = 0; i < SZ; i++) {
            if (rank(i) == SZ - 1)
                continue;
            int j = by_rank[rank(i) + 1];
            while (i + len < SZ && j + len < SZ && input[i + len] == input[j + len])
                len++;
            lcp[rank(i)] = len;
            if (len) --len;
        }
        lcp_rmq = sparse_table<int, smaller>(int(lcp.size()), lcp_cmp,
                [&](int i) { return lcp[i]; });
    }

    int rank(int i) const {
        if (i < SZ) {
            return rank_of.back()[i];
        } else switch (FILL) {
            case CYCLIC:  return rank_of.back()[i - SZ];
            case NEG_INF: return -1;
            case POS_INF: return SZ;
        }
    }

    int suffix_at_rank(int r) const { return by_rank[r]; }

    int longest_common_prefix(int i, int j) const {
        assert(0 <= i && i < SZ && 0 <= j && j < SZ);
        if (i == j) return SZ - i;
        i = rank(i);
        j = rank(j);
        if (i > j) swap(i, j);
        return lcp_rmq(i, j);
    }

    pair<int, int> ranks_with_prefix(int pos, int len) {
        assert(0 <= pos && pos + len <= SZ);
        auto less_than = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) < arg; };
        auto geq       = [&](int other_pos, int arg) { return longest_common_prefix(pos, other_pos) >= arg; };
        return make_pair(
            lower_bound(by_rank.begin(), by_rank.begin() + rank(pos), len, less_than) - by_rank.begin(),
            lower_bound(by_rank.begin() + rank(pos), by_rank.end(), len, geq) - by_rank.begin()
        );
    }
};

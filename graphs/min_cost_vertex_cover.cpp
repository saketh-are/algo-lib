#include <vector>
#include <limits>
#include <cstdint>

template<typename cost_t, typename sum_t>
sum_t min_cost_vertex_cover(int V, std::vector<cost_t> cost, std::vector<int64_t> adj) {
    const sum_t INF = std::numeric_limits<sum_t>::max() / 2;
    auto generate = [&](const int offset, const int ct) {
        std::vector<sum_t> v(1 << ct);

        for (int m = 1; m < int(v.size()); m++) {
            int i = __builtin_ctz(m);
            v[m] = v[m ^ (1 << i)] + cost[offset + i];
        }

        for (int m = 0; m < int(v.size()); m++) {
            int64_t need = 0;
            for (int i = 0; i < ct; i++) {
                if (!(m & (1 << i))) {
                    need |= adj[offset + i];
                }
            }
            need >>= offset;
            need &= (1 << ct) - 1;

            if ((m & need) != need) {
                v[m] = INF;
            }
        }

        for (int i = 0; i < ct; i++) {
            for (int m = 0; m < int(v.size()); m++) {
                if (!(m & (1 << i))) {
                    v[m] = std::min(v[m], v[m ^ (1 << i)]);
                }
            }
        }

        return v;
    };

    const int L = (V + 1) / 2;
    const int R = V / 2;

    auto f = generate(0, L);
    auto g = generate(L, R);

    sum_t ans = INF;

    for (int m = 0; m < (1 << L); m++) {
        int64_t need = 0;
        for (int i = 0; i < L; i++) {
            if (!(m & (1 << i))) {
                need |= adj[i];
            }
        }

        int left = int(need & ((1 << L) - 1));
        if ((m & left) != left) {
            continue;
        }

        int right = int(need >> L);
        ans = std::min(ans, f[m] + g[right]);
    }

    return ans;
}

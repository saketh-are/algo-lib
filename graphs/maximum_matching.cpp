int maximum_matching(const int N, const vector<array<int, 2>>& edges) {
    const int INF = 1e9 + 7;
    uniform_int_distribution<int> unif(1, INF - 1);

    int res = 0;
    for (int rep = 0; rep < 3; rep++) {
        matrix<modnum<INF>> m(N, 0, 0);
        for (auto [u, v] : edges) {
            if (u > v) swap(u, v);
            int eid = unif(rng);
            m(u, v) = eid;
            m(v, u) = -eid;
        }
        res = max(res, m.rank());
    }
    return res / 2;
}

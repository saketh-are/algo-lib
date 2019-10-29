    void collect_by_depth(vvi& d, int u, bool avoid_erased, int p = -1, int d0 = 0) const {
        if (d0 >= sz(d)) d.resize(d0 + 1);
        d[d0].push_back(u);
        for (int v : nbrs[u]) if (v != p && (!avoid_erased || !erased[v])) {
            collect_by_depth(d, v, avoid_erased, u, d0+1);
        }
    }

    friend bool rooted_iso(const tree<E>& a, int ra,
                           const tree<E>& b, int rb, bool avoid_erased = 1) {
        vvi abd, bbd;
        a.collect_by_depth(abd, ra, avoid_erased);
        b.collect_by_depth(bbd, rb, avoid_erased);
        if (sz(abd) != sz(bbd)) return false;

        int D = sz(abd);
        vi alb, blb;
        for (int d = D - 1; d >= 0; d--) {
            vi &al = abd[d], &bl = bbd[d];
            sort(all(al)), sort(all(bl));
            int L = sz(al); if (L != sz(bl)) return false;

            vector<pair<vi, int>> ac(L), bc(L);
            if (d + 1 < D) {
                const vi &na = abd[d+1], &nb = bbd[d+1];
                for (int i = 0; i < L; i++) {
                    ac[i].s = bc[i].s = i;
                    for (int n : a.nbrs[al[i]]) {
                        auto it = lb(all(na), n);
                        if (it != na.end() && *it == n)
                            ac[i].f.pb(alb[it - na.begin()]);
                    }
                    for (int n : b.nbrs[bl[i]]) {
                        auto it = lb(all(nb), n);
                        if (it != nb.end() && *it == n)
                            bc[i].f.pb(blb[it - nb.begin()]);
                    }
                    sort(all(ac[i].f)), sort(all(bc[i].f));
                }
            }
            sort(all(ac)), sort(all(bc));

            alb.resize(sz(ac)), blb.resize(sz(bc));
            if (d + 1 < D) for (int i = 0, j = 0; i < sz(ac); i++) {
                if (ac[i].f != bc[i].f) return false;
                if (i && (ac[i].f != ac[i-1].f)) j++;
                alb[ac[i].s] = blb[bc[i].s] = j;
            }
        }

        return true;
    }

    friend bool iso(const tree<E>& a, int ra,
                    const tree<E>& b, int rb, bool avoid_erased = 1) {
        int ca = a.centroids(ra, avoid_erased)[0];
        for (int cb : b.centroids(rb, avoid_erased))
            if (rooted_iso(a, ca, b, cb, avoid_erased))
                return true;
        return false;
    }
    friend bool iso(const tree<E>& a, const tree<E>& b) {
        return iso(a, 0, b, 0, 0);
    }

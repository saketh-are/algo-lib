/*
 * Static-to-dynamic transformation for decomposable searches.
 *
 * Requires that C(A union B).search(Q) = C(A).search(Q) + C(B).search(Q).
 * Computes C({ elts }).search(Q) in log(E).
 *
 * Supports insertion and absorption in amortized log(E).
 */
template<typename C, typename T, typename Q, typename V>
struct decomposable_search {
    int E;
    vector<T> elts;
    vector<C> collections;

    decomposable_search() : E(0) { }

    void insert(const T& elt) {
        elts.push_back(elt);
        if (__builtin_popcount(++E) == 1) collections.push_back(C());

        int grp = 31 - __builtin_clz(E&-E);
        collections[grp] = C(elts.end() - (E&-E), elts.end());
    }

    void absorb(decomposable_search<C, T, Q, V> o) {
        if (E < o.E) swap(*this, o);

        E += o.E;
        elts.insert(elts.end(), all(o.elts));

        int diff = 32 - __builtin_clz(E ^ (E - o.E));
        if (diff > collections.size()) collections.resize(diff);

        for (int i = diff - 1; i >= 0; i--) if (E & (1 << i)) {
            int start = E & (~0 << (i + 1)), end = start + (1 << i);
            collections[i] = C(elts.begin() + start, elts.begin() + end);
        }
    }

    V search(Q query) const {
        V res;
        for (int i = 0; i < collections.size(); i++) if (E & (1 << i)) {
            res += collections[i].search(query);
        }
        return res;
    }
};

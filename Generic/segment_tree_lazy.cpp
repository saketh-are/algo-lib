template<typename T, typename U> struct seg_tree_lazy {
    int S, H;

    T def;
    vector<T> value;

    U noop;
    vector<U> prop;

    seg_tree_lazy<T, U>(int _S, T _def = T(), U _noop = U()) {
        S = _S, def = _def, noop = _noop;
        assert(__builtin_popcount(S) == 1);
        H = 32 - __builtin_clz(S);

        value.resize(2 * S + 1, def);
        prop.resize(2 * S + 1, noop);
    }

    void apply(int i, U update) {
        value[i] = update(value[i]);
        if(i < S) prop[i] = prop[i] + update;
    }

    void rebuild(int i) {
        for (int l = i/2; l; l /= 2) {
            T combined = value[2*l] + value[2*l+1];
            value[l] = prop[l](combined);
        }
    }

    void propagate(int i) {
        for (int h = H; h > 0; h--) {
            int l = i >> h;
            if (prop[l] != noop) {
                apply(2*l, prop[l]);
                apply(2*l+1, prop[l]);
                prop[l] = noop;
            }
        }
    }

    void upd(int i, int j, U update) {
        i += S, j += S;
        propagate(i), propagate(j);

        for (int l = i, r = j; l <= r; l /= 2, r /= 2) {
            if((l&1) == 1) apply(l++, update);
            if((r&1) == 0) apply(r--, update);
        }

        rebuild(i), rebuild(j);
    }

    T query(int i, int j){
        i += S, j += S;
        propagate(i), propagate(j);

        T res_left = def, res_right = def;
        for(; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res_left = res_left + value[i++];
            if((j&1) == 0) res_right = value[j--] + res_right;
        }
        return res_left + res_right;
    }
};


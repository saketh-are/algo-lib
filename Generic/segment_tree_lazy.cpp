template<typename T, typename U> struct seg_tree_lazy {
    int S, H;

    T def, zero;
    vector<T> value;

    U noop;
    vector<U> prop;

    void build() {
        for (int i = S - 1; i > 0; i--) 
            value[i] = value[2 * i] + value[2 * i + 1];
    }

    void init(int _S) {
        for (S = 1, H = 1; S < _S; ) S *= 2, H++;
        value.resize(2*S, zero);
        prop.resize(2*S, noop);

        fill(value.begin() + S, value.begin() + S + _S, def);
        build();
    }

    seg_tree_lazy<T, U>(int _S, T _def = T(), T _zero = T(), U _noop = U()) {
        def = _def, zero = _zero, noop = _noop;
        init(_S);
    }

    void set_leaves(vector<T> &leaves) {
        copy(leaves.begin(), leaves.end(), value.begin() + S);
        fill(value.begin() + S + leaves.size(), value.end(), zero);
        build();
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
            if (memcmp(&prop[l], &noop, sizeof(U))) {
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

        T res_left = zero, res_right = zero;
        for(; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res_left = res_left + value[i++];
            if((j&1) == 0) res_right = value[j--] + res_right;
        }
        return res_left + res_right;
    }
};


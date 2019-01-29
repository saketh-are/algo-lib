template<typename T, typename U> struct seg_tree_lazy {
    int S, H;
    vector<T> value;
    vector<U> prop;
    vb dirty;
    bool push_down;

    seg_tree_lazy<T, U>(int _S, bool push_down = true) : push_down(push_down) {
        for (S = 1, H = 1; S < _S; S *= 2, H++);
        value.resize(2*S), dirty.resize(2*S), prop.resize(2*S);
    }

    void rebuild() {
        for (int i = S - 1; i > 0; i--)
            value[i] = prop[i](value[2 * i] + value[2 * i + 1]);
    }

    void rebuild(int i) {
        for (int l = i/2; l; l /= 2) {
            value[l] = prop[l](value[2*l] + value[2*l+1]);
        }
    }

    void apply(int i, U &update) {
        value[i] = update(value[i]);
        if (i < S) {
            prop[i] = prop[i] + update;
            dirty[i] = true;
        }
    }

    void propagate(int i) {
        for (int h = H; h > 0; h--) {
            int l = i >> h;
            if (dirty[l]) {
                apply(2*l, prop[l]);
                apply(2*l+1, prop[l]);
                prop[l] = U();
                dirty[l] = false;
            }
        }
    }

    void upd(int i, int j, U update) {
        i += S, j += S;
        if (push_down) propagate(i), propagate(j);

        for (int l = i, r = j; l <= r; l /= 2, r /= 2) {
            if ((l&1) == 1) apply(l++, update);
            if ((r&1) == 0) apply(r--, update);
        }
        rebuild(i), rebuild(j);
    }

    T query(int i, int j) {
        i += S, j += S;
        if (push_down) propagate(i), propagate(j);

        T res_left, res_right;
        for (; i <= j; i /= 2, j /= 2) {
            if ((i&1) == 1) res_left = res_left + value[i++];
            if ((j&1) == 0) res_right = value[j--] + res_right;
        }
        return res_left + res_right;
    }
};


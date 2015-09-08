template<typename T> struct seg_tree {
    int S;

    T def;
    vector<T> value;

    seg_tree<T>(int _S, T _def = T()) {
        S = _S, def = _def;
        value.resize(2*S+1, def);
    }

    void upd(int i, T v) {
        i += S;
        value[i] = v;
        while(i>1){
            i/=2;
            value[i] = value[2*i] + value[2*i+1];
        }
    }

    T query(int i, int j) {
        T res = def;
        for(i += S, j += S; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res = res + value[i++];
            if((j&1) == 0) res = res + value[j--];
        }
        return res;
    }
};


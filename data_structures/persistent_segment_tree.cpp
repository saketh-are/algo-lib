template<typename T> struct pseg_tree {
    T zero;

    int S;
    vector<vector<pair<int, T>>> value;

    pseg_tree<T>(int _S, T _zero = T()) : S(_S), zero(_zero) {
        value.resize(2*S+1);
    }

    void recompute(int op, int i) {
        const T& c1 = value[2 * i].empty() ? zero : value[2 * i].back().second;
        const T& c2 = value[2 * i + 1].empty() ? zero : value[2 * i + 1].back().second;
        value[i].emplace_back(op, c1 + c2);
    }

    void set_leaves(vector<T>& leaves) {
        for (int i = S; i < value.size(); i++)
            value[i].emplace_back(0, leaves[i]);
        for (int i = S - 1; i > 0; i--)
            recompute(0, i);
    }

    void upd(int i, T v, int op) {
        i += S;
        value[i].emplace_back(op, v);
        while(i>1){
            i/=2;
            recompute(op, i);
        }
    }

    T find(vector<pair<int, T>>& hist, int op) {
        auto it = upper_bound(all(hist), make_pair(op, zero ), [](auto& a, auto &b) {
            return a.first < b.first;
        });
        return it != hist.begin() ? prev(it)->second : zero;
    }

    T query(int i, int j, int op) {
        T res_left = zero, res_right = zero;
        for(i += S, j += S; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res_left = res_left + find(value[i++], op);
            if((j&1) == 0) res_right = find(value[j--], op) + res_right;
        }
        return res_left + res_right;
    }
};


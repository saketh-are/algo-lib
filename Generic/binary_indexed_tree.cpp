struct binary_indexed_tree{
    int S;
    vector<int> v;
    
    binary_indexed_tree(int _S){
        S = _S;
        v.resize(S+1);
    }

    int read(int i){
        int sum = 0;
        for(i++; i > 0; ){
            sum += v[i];
            i -= i & -i;
        }
        return sum;
    }

    void update(int i, int k){
        for(i++; i<v.size(); ){
            v[i] += k;
            i += i & -i;
        }
    }

    int read_range(int l, int r){
        return read(r) - read(l-1);
    }
};

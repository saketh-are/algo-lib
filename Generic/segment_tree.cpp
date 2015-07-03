template<typename T> struct seg_tree{
    int S;
    vector<T> arr;
    seg_tree<T>(int _S){
        assert(__builtin_popcount(_S) == 1);
        S = _S;
        arr.resize(2*S+1);
    }

    void upd(int i, T v){
        i += S;
        arr[i] = v;
        while(i>1){
            i/=2;
            arr[i] = arr[2*i] + arr[2*i+1];
        }
    }

    T query(int i, int j){
        T sum; 
        for(i += S, j += S; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) sum = sum + arr[i++];
            if((j&1) == 0) sum = sum + arr[j--];
        }
        return sum;
    }
};

struct seg_tree{
    struct node{
        int val;
        node(int _val = 0x7f7f7f7f){ 
            val = _val; 
        }
        node operator +(const node &y){
            return min(val, y.val);
        }
    };

    int S;
    vector<node> arr;
    seg_tree(int _S){
        assert(__builtin_popcount(_S) == 1);
        S = _S;
        arr.resize(2*S+1);
    }

    void upd(int i, node v){
        i += S;
        arr[i] = v;
        while(i>1){
            i/=2;
            arr[i] = arr[2*i] + arr[2*i+1];
        }
    }

    node query(int i, int j){
        node res; 
        for(i += S, j += S; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res = res + arr[i++];
            if((j&1) == 0) res = res + arr[j--];
        }
        return res;
    }
};


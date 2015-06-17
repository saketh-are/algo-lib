struct suffix_array{
    const int INF = 0x7f7f7f7f;

    int L, D;
    string str;
    vector<vector<int> > suff;
    vector<int> rank_of, at_rank;

    pair<pair<int, int>, int> __make_rep(int l, int i, int p){
        if(!l) return {{str[i], INF}, i};
        return {{suff[l-1][i], (i+p<L) ? suff[l-1][i+p] : INF}, i};
    }

    suffix_array(string _str){
        str = _str;
        L = str.size();
        D = 33 - __builtin_clz(L-1);
        suff.resize(D);

        vector<pair<pair<int, int>, int> > keys(L);
        for(int l=0; l<D; l++){
            for(int i=0; i<L; i++)
                keys[i] = __make_rep(l, i, 1<<(l-1));
            sort(keys.begin(), keys.end());
            
            suff[l].resize(L);
            for(int i=0, r=0; i<L; i++){
                if(i>0 && keys[i].first != keys[i-1].first) r++;
                suff[l][keys[i].second] = r;
            }
        }

        rank_of.resize(L);
        at_rank.resize(L);
        for(int i=0; i<L; i++){
            rank_of[i] = suff.back()[i];
            at_rank[rank_of[i]] = i;
        }
    }

    // check if the substring at [i, i+len) is equal the one at [j, j+len)
    bool check_eq(int i, int j, int len){
        if(max(i,j) + len > L) return false;
        
        for(int l=0; l<D; l++)
            if((len>>l)&1){
                if(suff[l][i] != suff[l][j]) return false; 
                i += 1<<l, j += 1<<l;
            }
        
        return true;
    }

    // find the interval of suffix ranks corresponding to instances of the substring at [i, i+len)
    pair<int, int> find_range(int i, int len){
        int left = rank_of[i]; 
        for(int lo=0, hi=left; lo<=hi; ){
            int mi = (lo + hi)/2;
            if(check_eq(i, at_rank[mi], len)){
                left = mi;
                hi = mi-1;
            }
            else lo = mi+1;
        }

        int right = rank_of[i];
        for(int lo=right, hi=L-1; lo<=hi; ){
            int mi = (lo + hi)/2;
            if(check_eq(i, at_rank[mi], len)){
                right = mi;
                lo = mi+1;
            }
            else hi = mi-1;
        }

        return make_pair(left, right);
    } 
};
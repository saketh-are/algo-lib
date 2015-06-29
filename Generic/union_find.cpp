struct union_find{
    vector<int> P, R, S;
    
    union_find(int N){
        P.resize(N), R.resize(N), S.resize(N);
        for(int i=0; i<N; i++){
            P[i] = i;
            R[i] = 0;
            S[i] = 1;
        }
    }
    
    int rep(int i){
        if(P[i] != i) P[i] = rep(P[i]);
        return P[i];
    }

    bool unio(int a, int b){
        a = rep(a), b = rep(b);
        if(a == b) return false;
        if(R[a] < R[b]) swap(a, b);
        P[b] = a;
        S[a] += S[b];
        if(R[a] == R[b]) R[a]++;
        return true;
    }
};


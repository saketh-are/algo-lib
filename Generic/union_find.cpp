struct union_find {
    vector<int> P, R, S;
    
    union_find (int N) {
        P.resize(N), R.resize(N, 0), S.resize(N, 1);
        for (int i = 0; i < N; i++){
            P[i] = i;
    }
    
    int rep(int i) {
        if(P[i] != i) P[i] = rep(P[i]);
        return P[i];
    }

    bool unio(int a, int b) {
        a = rep(a), b = rep(b);
        if(a == b) return false;
        if(R[a] < R[b]) swap(a, b);
        P[b] = a;
        S[a] += S[b];
        if(R[a] == R[b]) R[a]++;
        return true;
    }
};


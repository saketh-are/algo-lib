struct two_sat {
    int N;
    vector<vector<int>> impl;
 
    two_sat(int _N) {
        N = _N;
        impl.resize(2 * N);
    }
 
    void add_impl(int var1, bool neg1, int var2, bool neg2) {
        impl[2 * var1 + neg1].push_back(2 * var2 + neg2);
        impl[2 * var2 + !neg2].push_back(2 * var1 + !neg1);
    }
 
    void add_clause(int var1, bool neg1, int var2, bool neg2) {
        add_impl(var1, !neg1, var2, neg2);
    }
 
    void add_clause(int var1, bool neg1) {
        add_clause(var1, neg1, var1, neg1);
    }
 
    int V, L, C;
    stack<int> view;
 
    int dfs(int loc) {
        visit[loc] = V;
        label[loc] = L++;
 
        int low = label[loc];
        view.push(loc);
        in_view[loc] = true;
 
        for (int nbr : impl[loc]) {
            if(!visit[nbr]) low = min(low, dfs(nbr));
            else if(in_view[nbr]) low = min(low, label[nbr]);
        }
 
        if(low == label[loc]) {
            while (true) {
                int mem = view.top();
                comp[mem] = C;
                in_view[mem] = false;
                view.pop();
                if(mem == loc) break;
            }
            C++;
        }
 
        return low;
    }
 
    vector<int> visit, label, comp, in_view;
 
    void reset(vector<int> &v) {
        v.resize(2 * N);
        fill(v.begin(), v.end(), 0);
    }
 
    bool consistent() {
        V = 0, L = 0, C = 0;
        reset(visit), reset(label), reset(comp), reset(in_view);
 
        for (int i = 0; i < 2 * N; i++) {
            if(!visit[i]) {
                V++;
                dfs(i);
            }
        }
 
        for (int i = 0; i < N; i++)
            if(comp[2 * i] == comp[2 * i + 1]) {
                return false;
            }
 
        return true;
    }
};


template<typename T> T &quick_select(vector<T> &v, function<int(T&,T&)> comp, int l, int r, int k){
    int below = 0, same = 1;

    for(int i = l; i <= r - same; i++){
        int res = comp(v[i], v[r]);
        if(res == 0){
            swap(v[r-same], v[i]);
            same++;
            i--;
        }
        else if(res == -1){
            swap(v[l+below], v[i]);
            below++;
        }
    }
   
    if(below <= k && k < below+same) return v[r];
    if(below > k) return quick_select(v, comp, l, l+below-1, k);
    return quick_select(v, comp, l+below, r-same, k-below-same); 
}

template<typename T> T &quick_select(vector<T> &v, function<int(T&,T&)> comp, int k){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(v.begin(), v.end(), default_random_engine(seed));
    return quick_select(v, comp, 0, v.size()-1, k); 
}

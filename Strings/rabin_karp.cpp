namespace rabin_karp{
    const int MAXL = 200200;
    const ll SP = 3001, LP = 1e9 + 7;

    vector<ll> powers;
    void init(){
        powers.resize(MAXL);
        powers[0] = 1;
        for(int i=1; i<MAXL; i++)
            powers[i] = (SP * powers[i-1]) % LP;
    }

    vector<ll> compute(string str){
        vector<ll> hashes(str.size()+1);
        for(int i=1; i<=str.size(); i++)
            hashes[i] = (SP * hashes[i-1] + int(str[i-1])) % LP;
        return hashes;
    }

    ll read(vector<ll> &hashes, int i, int l){
        ll sub = (hashes[i] * powers[l]) % LP;
        return (hashes[i+l] - sub + LP) % LP;
    }
};


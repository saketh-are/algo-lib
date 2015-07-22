struct rabin_karp{
    static const int MAXL = 200200;
    static const ll SP = 3001, LP = 1e9 + 7;
    
    static vector<ll> powers;    
    static void init(){
        powers.resize(MAXL);        
        powers[0] = 1;
        for(int i=1; i<MAXL; i++)
            powers[i] = (SP * powers[i-1]) % LP;
    }
   
    vector<ll> hashes;
 
    rabin_karp(string str){
        hashes.resize(str.size()+1);
        for(int i=1; i<=str.size(); i++)
            hashes[i] = (SP * hashes[i-1] + int(str[i-1])) % LP;
    }

    ll read(int i, int l){
        ll sub = (hashes[i] * powers[l]) % LP;
        return (hashes[i+l] - sub + LP) % LP;
    }    
};
vector<ll> rabin_karp::powers;


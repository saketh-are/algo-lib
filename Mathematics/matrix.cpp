void add(ll &a, ll b){ a = (a+b) % MOD; }
ll sum(ll a, ll b){ return (a+b) % MOD; }
void mul(ll &a, ll b){ a = (a*b) % MOD; }
ll prod(ll a, ll b){ return (a*b) % MOD; }

struct mat{
    int N;
    vector<vector<ll> > dat;

    mat(int _N, ll diag = 0){
        N = _N;
        dat.resize(N);
        for(int i=0; i<N; i++){
            dat[i].resize(N);
            dat[i][i] = diag;
        }
    }

    mat operator *(mat &b){
        mat r(N);
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                for(int k=0; k<N; k++)
                    add(r[i][j], prod(dat[i][k], b[k][j]));
        return r;
    }
    
    mat pow(ll E){
        if(!E) return mat(N, 1);
        mat r = (*this * *this).pow(E/2);
        return E&1 ? r * *this : r;
    }
    
    vector<ll>& operator[](int i){
        return dat[i];
    }

    friend ostream& operator<<(ostream &os, mat &m){
        os << "{";
        for(int i=0; i<m.N; i++){
            if(i) os << "},\n ";
            os << "{";
            for(int j=0; j<m.N; j++){
                if(j) os << ", ";
                os << setw(10) << m[i][j] << setw(0);
            }
        }
        return os << "}}";
    }
};


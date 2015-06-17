const ll MOD = 1000000007;

void add(ll &a, ll b){ a = (a+b) % MOD; }
ll sum(ll a, ll b){ return (a+b) % MOD; }
void mul(ll &a, ll b){ a = (a*b) % MOD; }
ll prod(ll a, ll b){ return (a*b) % MOD; }

struct mat{
    int N;
    ll **dat;
    mat(int _N, ll diag = 0){
        N = _N;
        dat = (ll**) malloc(sizeof(ll*) * N);
        for(int i=0; i<N; i++){
            dat[i] = (ll*) calloc(N, sizeof(ll));
            dat[i][i] = diag;
        }
    }
    ~mat(){
        for(int i=0; i<N; i++) free(dat[i]);
        free(dat);
    }

    ll *&operator [](int i){
        return dat[i];
    }

    mat *operator *(mat &b){
        mat *r = new mat(N);
        for(int i=0; i<N; i++)
            for(int j=0; j<N; j++)
                for(int k=0; k<N; k++)
                    add(*r[i][j], prod(dat[i][k], b[k][j]));
        return r;
    }

    mat *pow(ll E){
        if(!E) return new mat(N, 1);
        mat *b = *this * *this;
        mat *p = b->pow(E>>1);
        delete b;
        if(E&1){ mat *q = *p * *this; delete p; return q; }
        return p;        
    }
};


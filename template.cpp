#include <bits/stdc++.h>
using namespace std;

#define TRACE(x) x
#define WATCH(x) TRACE(cout << #x" = " << x << endl)
#define WATCHR(a, b) TRACE(for (auto it=a; it!=b;) cout << *(it++) << " "; cout << endl)
#define WATCHC(V) TRACE({cout << #V" = "; WATCHR(V.begin(), V.end());})

#define sz(x) int((x).size())
#define all(x) (x).begin(), (x).end()

using ll = long long;
using vi = vector<int>;
using vvi = vector<vi>;
using vll = vector<ll>;
using vvll = vector<vll>;
using vb = vector<bool>;
using vd = vector<double>;
using vs = vector<string>;

template<typename T> void ckmin(T& a, const T& b) { a = min(a, b); }
template<typename T> void ckmax(T& a, const T& b) { a = max(a, b); }

template<class T> void print(ostream& o, T&& arg) { o << arg; }
template<class T, class ...Ts> void print(ostream& o, T&& first, Ts&&... rest) {
    o << first << ", "; print(o, forward<Ts&&>(rest)...);
}
template<class T, size_t... I>
void print(ostream& o, const T& t, index_sequence<I...>) {
    o << '['; print(o, get<I>(t)...); o << ']';
};
template<class ...T> ostream& operator<<(ostream& o, const tuple<T...>& t) {
    print(o, t, make_index_sequence<sizeof...(T)>{}); return o;
};
template<typename T1, typename T2>
ostream& operator<<(ostream& o, const pair<T1, T2>& t) {
    return o << "[" << t.first << ", " << t.second << "]";
};
template<typename T, size_t N>
ostream& operator<<(ostream& o, const array<T, N>& a) {
    o << '[';
    for (int i = 0; i < N; i++) { if (i) o << ", "; o << a[i]; }
    return o << ']';
};

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);

    return 0;
}


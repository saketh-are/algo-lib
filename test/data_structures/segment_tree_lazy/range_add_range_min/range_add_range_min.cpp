// Problem: https://codeforces.com/contest/52/problem/C

#include <cstdio>
#include <vector>
#include <cstdint>
#include <functional>
#include <cstring>
using namespace std;

// {{{ data_structures/segment_tree_lazy }}}

int main() {
    int N;
    scanf("%d", &N);

    using std_min_type = const int64_t&(*)(const int64_t&, const int64_t&);

    segment_tree_lazy<int64_t, int64_t, std_min_type, plus<int64_t>, plus<int64_t>>
        st(N, 1e12, 0, min<int64_t>, plus<int64_t>(), plus<int64_t>());

    st.assign([](int i) { int v; scanf("%d", &v); return v; });

    int Q;
    scanf("%d\n", &Q);

    char s[256];

    for (int q = 0; q < Q; q++) {
        fgets(s, 256, stdin);

        char *p = strtok(s, " ");
        int lf = atoi(p);

        p = strtok(NULL, " ");
        int rg = atoi(p);

        p = strtok(NULL, " ");
        if (p != NULL) {
            int v = atoi(p);
            if (lf <= rg) {
                st.apply_update(lf, rg + 1, v);
            } else {
                st.apply_update(lf, N, v);
                st.apply_update(0, rg + 1, v);
            }
        } else {
            int64_t res;
            if (lf <= rg)
                res = st.accumulate(lf, rg + 1);
            else
                res = min(st.accumulate(lf, N), st.accumulate(0, rg + 1));
            printf("%jd\n", res);
        }
    }

    return 0;
}


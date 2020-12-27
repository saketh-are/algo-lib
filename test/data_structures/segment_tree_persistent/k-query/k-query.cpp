// Problem: https://www.spoj.com/problems/KQUERY/

#include <cstdio>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>
using namespace std;

// {{{ data_structures/segment_tree_persistent }}}

int main() {
    int N;
    scanf("%d", &N);

    vector<int> a(N);
    for (int i = 0; i < N; i++)
        scanf("%d", &a[i]);

    segment_tree_persistent<int, plus<int>> st(N, 0, plus<int>());

    vector<int> insert_order(N);
    iota(insert_order.begin(), insert_order.end(), 0);
    sort(insert_order.begin(), insert_order.end(), [&](int i, int j) { return a[i] > a[j]; });

    for (int i : insert_order)
        st.assign(i, 1, -a[i]);

    int Q;
    scanf("%d", &Q);

    for (int q = 0; q < Q; q++) {
        int i, j, k;
        scanf("%d %d %d", &i, &j, &k);

        printf("%d\n", st.accumulate(i - 1, j, -k));
    }

    return 0;
}


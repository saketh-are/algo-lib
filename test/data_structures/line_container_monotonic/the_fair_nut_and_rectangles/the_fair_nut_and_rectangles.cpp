// Problem: https://codeforces.com/contest/1083/problem/E

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// {{{ data_structures/line_container_monotonic }}}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N;
    cin >> N;

    struct rectangle {
        int x, y;
        int64_t c;
    };

    vector<rectangle> rect(N);

    for (auto &[x, y, c] : rect)
        cin >> x >> y >> c;

    sort(rect.begin(), rect.end(), [](rectangle a, rectangle b) {
        return a.x < b.x;
    });

    line_container_monotonic<int64_t> lc;

    lc.insert_line(0, 0);

    int64_t ans = 0;

    size_t position = 0;

    for (auto [x, y, c] : rect) {
        int64_t best = int64_t(x) * y - c + lc.get_maximum_monotonic(-y, position);

        ans = max(ans, best);

        lc.insert_line(x, best);
    }

    cout << ans << endl;

    return 0;
}

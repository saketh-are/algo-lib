// Problem: https://judge.yosupo.jp/problem/two_sat

#include <cstdlib>

// {{{ graphs/two_sat.cpp }}}

int main() {
    char c;
    for (int i = 0; i < 4; i++)
        scanf(" %c", &c);

    int N, M;
    scanf("%d %d", &N, &M);

    two_sat ts(N);

    for (int i = 0; i < M; i++) {
        int a, b;
        scanf("%d %d %c", &a, &b, &c);

        int x = 2 * (abs(a) - 1) + int(a > 0);
        int y = 2 * (abs(b) - 1) + int(b > 0);

        ts.constraint_at_least_one(x, y);
    }

    std::vector<bool> assignment = ts.solve();

    if (assignment.empty()) {
        printf("s UNSATISFIABLE\n");
    } else {
        printf("s SATISFIABLE\n");
        printf("v");

        for (int i = 0; i < N; i++) {
            printf(" ");
            if (!assignment[i])
                printf("-");
            printf("%d", i + 1);
        }

        printf(" 0\n");
    }
}

// {{{ graphs/strongly_connected_components.cpp }}}
// {{{ graphs/digraph_reachability.cpp }}}

#include <vector>
#include <string>

struct two_sat {
    std::vector<std::vector<int>> graph;

    two_sat(int SZ) : graph(2 * SZ) {}

    int new_var() {
        graph.push_back({});
        graph.push_back({});
        return int(graph.size()) - 1;
    }

    static inline int neg(int a) { return a^1; }

    void implies(int a, int b) {
        if (a < 0 || b < 0) return;
        graph[a].push_back(b);
        graph[neg(b)].push_back(neg(a));
    }

    void assign(int a, int v) {
        if (v) implies(neg(a), a);
        else implies(a, neg(a));
    }
    void constrain_equal(int a, int b) {
        implies(a, b);
        implies(neg(a), neg(b));
    }
    void constrain_not_equal(int a, int b) {
        implies(a, neg(b));
        implies(neg(a), b);
    }
    void constrain_at_most_one(int a, int b) {
        implies(a, neg(b)); // contrapositive is automatic
    }
    void constraint_at_least_one(int a, int b) {
        implies(neg(a), b); // contrapositive is automatic
    }

    int make_or(int a, int b) {
        if (a < 0) return b;
        if (b < 0) return a;
        int v = new_var();
        implies(a, v);
        implies(b, v);
        return v;
    }

    int make_and(int a, int b) {
        if (a < 0) return b;
        if (b < 0) return a;
        int v = new_var();
        implies(v, a);
        implies(v, b);
        return v;
    }

    std::vector<bool> solve() const {
        scc s = strongly_connected_components(graph);
        std::vector<bool> assignment(int(graph.size()) / 2);

        for (int v = 0; v < int(graph.size()); v += 2) {
            if (s.label[v] == s.label[v + 1])
                return {};

            assignment[v / 2] = s.label[v] > s.label[v + 1];
        }
        return assignment;
    }

    // (2 * MAXV)^2 / machine word size
    template<int MAXV>
    std::string classify() const {
        auto reach = digraph_reachability<2 * MAXV>(graph);
        std::string res(int(graph.size()), '?');
        for (int v = 0; v < int(graph.size()); v++) {
            bool n0 = reach[v][neg(v)];
            bool n1 = reach[neg(v)][v];
            if (n0 && n1) return "";
            res[v] = n0 ? '0' : n1 ? '1' : '?';
        }
        return res;
    }
};

/*
 * Segment tree supporting element modification and range sum queries.
 */
segment_tree st(N, 0, plus<int>());

/*
 * Segment tree supporting element modification and range min queries.
 */
segment_tree st(N, INT_MAX, [](int a, int b){ return min(a, b); });

/*
 * Segment tree for strings of balanced parentheses.
 *      - '(' = {  1,  0, 1 }
 *      - ')' = { -1, -1, 0 }
 */
struct node { int sum, lo, hi; };
auto nn = [](node a, node b) {
    return node{ a.sum + b.sum, min(a.lo, a.sum + b.lo), max(a.hi, a.sum + b.hi) };
};
segment_tree st(N, node{0,0,0}, nn);

/*
 * Segment tree supporting:
 *     - Set value at index i
 *     - Add value to each element in [i, j)
 *     - Return the first minimum in [i, j) and its index
 */
using nv_t = ll;
struct n { nv_t lo; int ix; };
using u = nv_t;
auto nn = [](n a, n b) { return n{ min(a.lo, b.lo), a.lo <= b.lo ? a.ix : b.ix }; };
auto uu = plus<nv_t>();
auto un = [](u U, n N) { N.lo += U; return N; };

segment_tree_lazy st(N, n{INF, -1}, 0ll, nn, uu, un);
st.set_leaves([&](int i){ return n{0ll, i}; });

/*
 * Segment tree supporting:
 *      - Set value at index i
 *      - Add value to each element in [i, j)
 *      - Compute the sum of the elements in [i, j)
 */
using nv_t = ll;
struct node { int elts; nv_t x; };
auto nn = [](node a, node b) { return node{a.elts + b.elts, a.x + b.x }; };
struct update { nv_t x; };
auto uu = [](update a, update b) { return update{a.x + b.x}; };
auto un = [](update u, node n) { return node{n.elts, n.x + u.x * n.elts }; };

segment_tree_lazy st(N, node{0,0}, update{0}, nn, uu, un);
st.set_leaves([](int i){ return node{1,0}; });

/*
 * Segment tree supporting:
 *     - Set pair of values at index i
 *     - Add pair of values to each pair in [i, j)
 *     - Compute the sum of the pairs in [i, j)
 */
using nv_t = ll;
struct node { int elts; nv_t x, y; };
auto nn = [](node a, node b) { return node{a.elts + b.elts, a.x + b.x, a.y + b.y }; };
struct update { nv_t x, y; };
auto uu = [](update a, update b) { return update{a.x + b.x, a.y + b.y}; };
auto un = [](update u, node n) { return node{n.elts, n.x + u.x * n.elts, n.y + u.y * n.elts }; };

segment_tree_lazy st(N, node{0,0,0}, update{0,0}, nn, uu, un);
st.set_leaves([](int i){ return node{1,0,0}; });

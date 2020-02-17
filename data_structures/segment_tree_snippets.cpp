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
 * Flip range, count zeros and ones
 */
using nv_t = int;
struct node { nv_t off, on; };
using upd = bool;
auto nn = [](node a, node b) { return node{a.off+b.off, a.on+b.on}; };
auto uu = [](upd a, upd b) { return a ^ b; };
auto un = [](upd a, node b) { return a ? node{b.on, b.off} : b; };

segment_tree_lazy st(N, node{0,0}, upd{0}, nn, uu, un);
st.set_leaves([](int i){ return node{1, 0}; });

/*
 * Add to range, find min
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
 * Add to range, get sum
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
 * Add to range, get sum (2D vectors)
 */
using nv_t = ll;
struct node { int elts; nv_t x, y; };
auto nn = [](node a, node b) { return node{a.elts + b.elts, a.x + b.x, a.y + b.y }; };
struct update { nv_t x, y; };
auto uu = [](update a, update b) { return update{a.x + b.x, a.y + b.y}; };
auto un = [](update u, node n) { return node{n.elts, n.x + u.x * n.elts, n.y + u.y * n.elts }; };

segment_tree_lazy st(N, node{0,0,0}, update{0,0}, nn, uu, un);
st.set_leaves([](int i){ return node{1,0,0}; });

/*
 * Add to "active" elements in range, flip active status in range, get sum
 */
struct box {
    int open = 0, closed = 0;
    ll total = 0;
};
auto bb = [](box a, box b) {
    return box{a.open + b.open, a.closed + b.closed, a.total + b.total};
};
struct update {
    bool flip = 0;
    ll add_to_open   = 0;
    ll add_to_closed = 0;
};
auto uu = [](update a, update b) {
    update r;
    r.flip = a.flip ^ b.flip;
    if (b.flip) swap(a.add_to_open, a.add_to_closed);
    r.add_to_open = a.add_to_open + b.add_to_open;
    r.add_to_closed = a.add_to_closed + b.add_to_closed;
    return r;
};
auto ub = [](update u, box b) {
    b.total += b.open * u.add_to_open + b.closed * u.add_to_closed;
    if (u.flip) swap(b.open, b.closed);
    return b;
};

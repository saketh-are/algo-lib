// {{{ data_structures/line }}}

#include <cassert>
#include <set>

bool compare_on_intersection = false;

template<typename T>
bool operator < (const line<T> &p, const line<T> &q) {
    if (compare_on_intersection)
        return p.intersects_next < q.intersects_next;

    return p.a != q.a ? p.a < q.a : p.b < q.b;
}

template<typename T>
struct line_container : std::set<line<T>> {
    using typename std::set<line<T>>::iterator;

    bool set_boundary(iterator left, iterator right) {
        if (right == this->end()) {
            left->intersects_next = line<T>::infinity;
            return false;
        }

        left->intersects_next = left->compute_intersection(*right);
        return left->intersects_next >= right->intersects_next;
    }

    bool is_never_maximal(iterator y) {
        return y != this->begin() && set_boundary(prev(y), y);
    }

    // insert the line f(x) = a * x + b
    void insert_line(T a, T b) {
        auto [it, inserted] = this->insert({ a, b });

        if (!inserted)
            return;

        while (set_boundary(it, next(it)))
            this->erase(next(it));

        if (is_never_maximal(it)) {
            it = this->erase(it);
            set_boundary(prev(it), it);
        }

        while (it != this->begin() && is_never_maximal(prev(it))) {
            this->erase(prev(it));
            set_boundary(prev(it), it);
        }
    }

    // returns the maximum value at x among all inserted lines
    T get_maximum(T x) {
        assert(!this->empty());
        compare_on_intersection = true;
        T res = this->lower_bound({ 0, 0, x })->evaluate(x);
        compare_on_intersection = false;
        return res;
    }
};

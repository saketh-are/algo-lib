// {{{ data_structures/line }}}

#include <vector>
#include <cassert>

template<typename T>
struct line_container_monotonic : std::vector<line<T>> {
    /* Inserts the line f(x) = a * x + b.
     * a must be non-decreasing across calls.
     */
    void insert_line(T a, T b) {
        line<T> ins = { a, b };

        if (!this->empty()) {
            auto it = this->rbegin();

            if (it->a > a)
                assert(false);

            if (it->a == a && it->b >= b)
                return;

            it->intersects_next = it->compute_intersection(ins);

            while (this->size() >= 2 && next(it)->intersects_next >= it->intersects_next) {
                this->pop_back();
                it = next(it);
                it->intersects_next = it->compute_intersection(ins);
            }
        }

        this->push_back(ins);
    }

    /* Returns the maximum value at x among all inserted lines.
     */
    T get_maximum(T x) const {
        assert(!this->empty());
        return std::lower_bound(this->begin(), this->end(), x, [](const line<T> &l, int _x) {
            return l.intersects_next < _x;
        })->evaluate(x);
    }

    /* Returns the maximum value at x among all inserted lines.
     * Total runtime complexity is linear over sequential calls made
     * with non-decreasing x if position is not modified externally.
     */
    T get_maximum_monotonic(T x, size_t &position) const {
        assert(!this->empty());

        if (position > this->size())
            position = this->size();

        while (position > 0 && (*this)[position - 1].intersects_next >= x)
            position--;

        while (x > (*this)[position].intersects_next)
            position++;

        return (*this)[position].evaluate(x);
    }
};

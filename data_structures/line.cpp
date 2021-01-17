#include <functional>
#include <limits>

template<typename T, bool T_integral = std::is_integral<T>::value>
struct line {
    static constexpr T infinity = std::numeric_limits<T>::has_infinity ?
        std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();

    T a, b;
    mutable T intersects_next = infinity;

    T evaluate(T x) const {
        return a * x + b;
    }

    static T div(const T &a, const T &b) {
        if constexpr (T_integral)
            return a / b - ((a ^ b) < 0 && (a % b));
        else
            return a / b;
    }

    T compute_intersection(const line<T> &o) const {
        if (a == o.a) return b > o.b ? infinity : -infinity;
        return div(o.b - b, a - o.a);
    }
};

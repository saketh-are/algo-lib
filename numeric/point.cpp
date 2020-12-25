#include <iostream>
#include <vector>

template<typename T>
struct point {
    T x, y;
    point() : x(0), y(0) {}
    point(T _x, T _y) : x(_x), y(_y) {}
    friend std::istream& operator >> (std::istream& i, point& p) { return i >> p.x >> p.y; }
    friend std::ostream& operator << (std::ostream& o, const point& p) {
        return o << "(" << p.x << ", " << p.y << ")";
    }

    T norm() const { return x * x + y * y; }
    double len() const { return sqrt(norm()); }
    double ang() const { return atan2(y, x); }
    point<T> conj() const { return { x, -y }; }

    point& operator += (const point& o) { x += o.x; y += o.y; return *this; }
    point& operator -= (const point& o) { x -= o.x; y -= o.y; return *this; }
    point& operator *= (const T& c) { x *= c; y *= c; return *this; }
    point& operator /= (const T& c) { x /= c; y /= c; return *this; }
    friend point operator + (const point& a, const point& b) { return point(a) += b; }
    friend point operator - (const point& a, const point& b) { return point(a) -= b; }
    friend point operator * (const point& a, const T& c) { return point(a) *= c; }
    friend point operator * (const T& c, const point& a) { return point(a) *= c; }
    friend point operator / (const point& a, const T& c) { return point(a) /= c; }

    bool operator == (const point& o) const { return x == o.x && y == o.y; }
    bool operator != (const point& o) const { return x != o.x || y != o.y; }
    bool operator < (const point& o) const { return (x != o.x) ? x < o.x : y < o.y; }
    bool operator > (const point& o) const { return (x != o.x) ? x > o.x : y > o.y; }
    bool operator <= (const point& o) const { return !(*this > o); }

    friend T dot(const point& a, const point& b) { return a.x * b.x + a.y * b.y; }
    friend T cross(const point& a, const point& b) { return a.x * b.y - a.y * b.x; }
    friend T cross(const point& p, const point& a, const point& b) { return cross(a - p, b - p); }

    friend point<T> operator * (const point& a, const point& b) {
        return { a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x };
    }
    friend point<T> operator / (const point& a, const point& b) {
        return a * b.conj() / b.norm();
    }

    point dir() const { return point(*this) / len(); }
    point rotate(const double& ang) const { return *this * point(cos(ang), sin(ang)); }

    double dist(const point& a) const { return (a - *this).len(); }
    double dist(const point& a, const point& b) {
        return abs(cross(*this, a, b)) / a.dist(b);
    }
    point reflect(const point& a, const point& b) const {
        return a + ((*this - a) / (b - a)).conj() * (b - a);
    }
    point foot(const point& a, const point& b) const {
        return (*this + this->reflect(a, b)) / 2;
    }
    friend point extension(const point& a, const point& b, const point& c, const point& d) {
        T u = cross(a, b, c), v = cross(a, b, d);
        return (d * u - c * v) / (u - v);
    }

    using polygon = std::vector<point<T>>;

    friend polygon seg_intersect(point a, point b, point c, point d) {
        if (a > b) swap(a, b);
        if (c > d) swap(c, d);

        T a1 = cross(a, b, c), a2 = cross(a, b, d);
        if (a1 > a2) swap(a1, a2);
        if (!(a1 <= 0 && a2 >= 0)) return {};

        if (a1 == 0 && a2 == 0) {
            if (cross(a, c, d) != 0) return {};
            point<T> x1 = max(a, c), x2 = min(b, d);
            if (x1 > x2) return {};
            if (x1 == x2) return { x1 };
            return { x1, x2 };
        }

        point<T> z = extension(a, b, c, d);
        if (a <= z && z <= b) return { z };
        return {};
    }

    friend std::ostream& operator << (std::ostream& o, const polygon& poly) {
        o << "{";
        for (auto pt : poly) o << " " << pt;
        return o << " }";
    }

    enum class Classification { in, out, on };
    Classification classify(const polygon& p) const {
        bool ans = 0;
        for (int i = 0; i < int(p.size()); i++) {
            point<T> a = p[i], b = p[(i + 1) % p.size()];
            if (cross(a, b, *this) == 0 && min(a, b) <= *this && *this <= max(a, b))
                return Classification::on;
            if (a.y > b.y) swap(a, b);
            if (a.y <= y && y < b.y && cross(*this, a, b) > 0) ans ^= 1;
        }
        return ans ? Classification::in : Classification::out;
    }

    friend polygon convex_hull(const std::vector<point>& pts) {
        point pivot = *min_element(all(pts));
        auto sorted = pts;
        sort(all(sorted), [&pivot](const point& p, const point& q) {
            T cp = cross(pivot, p, q);
            if (cp != 0) return cp > 0;
            return pivot.dist(p) < pivot.dist(q);
        });

        int j = 0;
        polygon res(pts.size());
        for (const point& p : sorted) {
            while (j > 1 && cross(res[j - 2], res[j - 1], p) <= 0)
                j--;
            res[j++] = p;
        }
        res.erase(res.begin() + j, res.end());
        return res;
    }

    // twice the signed area
    friend T area(const polygon& p) {
        T a = 0;
        for (int i = 0; i < int(p.size()); i++)
            a += cross(p[i], p[i+1 < sz(p) ? i+1 : 0]);
        return a;
    }

    // sorts CCW for vectors in the same half-plane
    template <typename U> static int sign(const U &v) { return (v > 0) - (v < 0); }
    friend int compare_by_angle(const point &a, const point &b) { return sign(cross(b, a)); }
    friend int compare_by_angle(const point &a, const point &b, const point &origin) { return sign(cross(origin, b, a)); }

    // sorts by angle CCW from the positive x-axis
    template <typename I> friend void sort_by_angle(I first, I last, const point<T> &origin = point<T>{}) {
        first = partition(first, last, [&](const point<T> &p) { return p == origin; });
        auto pivot = partition(first, last, [&](const point<T> &p) { return p > origin; });
        auto compare = [&](const point<T> &l, const point<T> &r) { return compare_by_angle(l, r, origin) < 0; };
        sort(first, pivot, compare);
        sort(pivot, last, compare);
    }
};

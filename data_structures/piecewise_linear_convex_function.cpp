template<typename T> struct PWLC {
    T y0;
    max_heap<pair<T, T>> left;
    min_heap<pair<T, T>> right;
    T left_offset = 0, right_offset = 0;

    PWLC(T _y0 = 0) : y0(_y0) {}

    /* __ : y = alpha */
    static PWLC constant(T alpha) { return PWLC(alpha); }
    /* \_ : y = max(0, beta * (x - x0)) */
    static PWLC decreasing(T x0, T beta = -1) { assert(beta <= 0); PWLC f; f.left.push({ x0, -beta }); return f; }
    /* _/ : y = max(0, beta * (x - x0)) */
    static PWLC increasing(T x0, T beta =  1) { assert(beta >= 0); PWLC f; f.right.push({ x0, beta }); return f; }
    /* \/ : y = beta * abs(x - x0) */
    static PWLC abs(T x0, T beta = 1) { return decreasing(x0, -beta) + increasing(x0, beta); }

    /* f'(x) = f(x + c) */
    void shift(int c) { left_offset -= c; right_offset -= c; }
    /* \_/ => \__ : f'(x') = min_{x <= x'} f(x) */
    void prefix_min() { while (!right.empty()) right.pop(); }
    /* \_/ => __/ : f'(x') = min_{x >= x'} f(x) */
    void suffix_min() { while (!left.empty()) left.pop(); }
    /* \_/ => \__/ : f'(x') = min_{dx in [xl, xr]} f(x' + dx) */
    void range_min(T xl, T xr) {
        assert(xl <= xr);
        if (xl > 0) shift(xl), tie(xl, xr) = make_pair(0, xr - xl);
        if (xr < 0) shift(xr), tie(xl, xr) = make_pair(xl - xr, 0);
        left_offset -= xr;
        right_offset -= xl;
    }

    T minimum() const { return y0; }
    T left_arg_min() const { return left.empty() ? numeric_limits<T>::min() : (left.top().first + left_offset); }
    T right_arg_min() const { return right.empty() ? numeric_limits<T>::max() : (right.top().first + right_offset); }

    void operator+=(PWLC o) {
        y0 += o.y0;
        for (; !o.left.empty(); o.left.pop()) {
            T x = o.left.top().first + o.left_offset;
            T beta_change = o.left.top().second;

            if (x <= right_arg_min()) {
                left.push({ x - left_offset, beta_change });
            } else {
                T x0 = right_arg_min();
                y0 += (x - x0) * beta_change;
                right.push({ x - right_offset, beta_change });
                for (T beta = beta_change; beta > 0; ) {
                    T next_change = right.top().second;
                    right.pop();
                    if (next_change >= beta) {
                        left.push({ x0 - left_offset, beta });
                        if (next_change > beta) right.push({ x0 - right_offset, next_change - beta });
                        beta = 0;
                    } else {
                        beta -= next_change;
                        y0 -= beta * (right_arg_min() - x0);
                        x0 = right_arg_min();
                    }
                }
            }
        }
        for (; !o.right.empty(); o.right.pop()) {
            T x = o.right.top().first + o.right_offset;
            T beta_change = o.right.top().second;

            if (x >= left_arg_min()) {
                right.push({ x - right_offset, beta_change });
            } else {
                T x0 = left_arg_min();
                y0 += (x0 - x) * beta_change;
                left.push({ x - left_offset, beta_change });
                for (T beta = beta_change; beta > 0; ) {
                    T next_change = left.top().second;
                    left.pop();
                    if (next_change >= beta) {
                        right.push({ x0 - right_offset, beta });
                        if (next_change > beta) left.push({ x0 - left_offset, next_change - beta });
                        beta = 0;
                    } else {
                        beta -= next_change;
                        y0 -= beta * (x0 - left_arg_min());
                        x0 = left_arg_min();
                    }
                }
            }
        }
    }
    friend PWLC operator+(const PWLC &a, const PWLC &b) { PWLC f(a); f += b; return f; }
};

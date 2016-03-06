struct upper_envelope {
    typedef pair<ll, ll> line;

    map<line, ll> hull;
    map<ll, line> scope;

    map<ll, line> elts;

    // x-coordinate of the intersection point, rounded down
    ll intersect(line a, line b) {
        if (a.first > b.first) swap(a, b);
        ll numer = a.second - b.second, denom = b.first - a.first;
        if (numer < 0) numer -= denom - 1;
        return numer / denom;
    }

    void move_limit(line key, ll limit) {
        scope.erase(hull[key]);
        hull[key] = limit;
        scope[limit] = key;
    }

    void remove(line rem) {
        scope.erase(hull[rem]);
        elts.erase(rem.first);

        auto it = hull.lower_bound(rem);

        if (it == hull.begin()) {
        } else if (next(it) == hull.end()) {
            move_limit(prev(it)->first, LLONG_MAX);
        } else {
            move_limit(prev(it)->first, intersect(prev(it)->first, next(it)->first));
        }

        hull.erase(rem);
    }

    void insert(ll slope, ll intercept) {
        if (elts.count(slope)) {
            if (elts[slope].second >= intercept) return;
            remove(elts[slope]);
            return insert(slope, intercept);
        }

        line ins = make_pair(slope, intercept);
        auto it = hull.lower_bound(make_pair(slope, LLONG_MIN));

        if (it != hull.end() && it != hull.begin()) {
            line succ = it->first, pred = prev(it)->first;
            if (intersect(pred, ins) >= intersect(pred, succ)) return;
        }

        while (it != hull.end() && next(it) != hull.end()) {
            line succ1 = it->first, succ2 = next(it)->first;
            if (intersect(ins, succ1) >= intersect(ins, succ2)) {
                it++;
                remove(succ1);
            } else break;
        } 

        while (it != hull.begin() && prev(it) != hull.begin()) {
            line pred1 = prev(it)->first, pred2 = prev(prev(it))->first;
            if (intersect(pred2, pred1) >= intersect(pred2, ins)) {
                remove(pred1);
            } else break;
        }

        if (it != hull.begin()) {
            line pred = prev(it)->first;
            move_limit(pred, intersect(pred, ins));
        }

        ll xc = (it == hull.end()) ? LLONG_MAX : intersect(ins, it->first);

        hull[ins] = xc;
        scope[xc] = ins;
        elts[ins.first] = ins;
    }

    ll maximum(ll x) {
        if (!hull.size()) return LLONG_MIN;
        line upper = scope.lower_bound(x)->second;
        return upper.first * x + upper.second;
    }
};


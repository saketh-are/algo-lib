namespace rectangles {
    // Represents the rectangle [x1, x2] x [y1, y2]
    struct rect {
        int x1, y1, x2, y2;
    };

    struct y_interval {
        int width, cover, ct;
        y_interval(int width = 0, int cover = 0) : width(width), cover(cover), ct(0) {}
        y_interval operator + (const y_interval& o) const {
            return y_interval(width + o.width, (ct > 0 ? width : cover) + (o.ct > 0 ? o.width : o.cover));
        }
    };

    struct update_ct {
        int ct;
        update_ct(int ct = 0) : ct(ct) {}
        update_ct operator + (const update_ct& o) const {
            return update_ct(ct + o.ct);
        }
        y_interval operator() (const y_interval& i) const {
            auto res = i;
            res.ct += ct;
            return res;
        }
    };

    struct v_boundary {
        int x, sign, y1i, y2i;
        bool operator < (const v_boundary& o) const {
            return x < o.x;
        }
    };

    ll area_of_union(const vector<rect>& rects) {
        vi y_vals;
        for (auto& rect : rects) {
            y_vals.push_back(rect.y1);
            y_vals.push_back(rect.y2);
        }
        sort(all(y_vals));
        y_vals.erase(unique(all(y_vals)), y_vals.end());

        seg_tree_lazy<y_interval, update_ct> st(y_vals.size() - 1, false);
        for (int i = 0; i + 1 < y_vals.size(); i++)
            st.value[st.S + i] = y_interval(y_vals[i+1] - y_vals[i]);
        st.rebuild();

        vector<v_boundary> events;
        for (auto &rect : rects) {
            int y1i = lower_bound(all(y_vals), rect.y1) - y_vals.begin();
            int y2i = lower_bound(all(y_vals), rect.y2) - y_vals.begin();
            events.push_back({ rect.x1,  1, y1i, y2i - 1 });
            events.push_back({ rect.x2, -1, y1i, y2i - 1 });
        }
        sort(all(events));

        ll ans = 0;
        for (int e = 0; e < events.size(); e++) {
            if (e) ans += (events[e].x - events[e-1].x) * 1ll * st.query(0, y_vals.size() - 2).cover;
            st.upd(events[e].y1i, events[e].y2i, update_ct(events[e].sign));
        }
        return ans;
    }
};


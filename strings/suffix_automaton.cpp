template<int MIN_CHAR, int SIGMA>
struct suffix_automaton {
    struct state {
        int len = 0;
        int suffix_link = -1;
        int first_end_pos = -1;
        array<int, SIGMA> transitions;
        state() { transitions.fill(-1); }
    };

    int SZ, last;
    vector<state> data;
    vector<bool> is_clone;

    const state& operator [] (int loc) const { return data[loc]; }

    int create_state(state s = {}) {
        data.push_back(s);
        is_clone.push_back(s.len != 0);
        return int(data.size()) - 1;
    }

    suffix_automaton() : SZ(0) { last = create_state(); }

    template<typename I>
    suffix_automaton(I begin, I end) : suffix_automaton() {
        for (I iter = begin; iter != end; iter++)
            __append(*iter);
        __initialize_auxiliary_data();
    }

    void __append(int c) {
        assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);
        c -= MIN_CHAR;

        int p = last;

        last = create_state();
        data[last].first_end_pos = SZ;
        data[last].len = ++SZ;

        while (p != -1 && data[p].transitions[c] == -1) {
            data[p].transitions[c] = last;
            p = data[p].suffix_link;
        }

        if (p == -1) {
            data[last].suffix_link = 0;
            return;
        }

        int q = data[p].transitions[c];
        if (data[q].len - data[p].len == 1) {
            data[last].suffix_link = q;
            return;
        }

        int pc = create_state(data[q]);
        data[pc].len = data[p].len + 1;

        while (p != -1 && data[p].transitions[c] == q) {
            data[p].transitions[c] = pc;
            p = data[p].suffix_link;
        }

        data[q].suffix_link = pc;
        data[last].suffix_link = pc;
    }

    vector<int> ct_end_pos;
    vector<vector<int>> children; // suffix link tree

    void __initialize_auxiliary_data() {
        children.resize(data.size());
        vector<vector<int>> states_with_length(SZ + 1);
        for (int loc = 1; loc < int(data.size()); loc++) {
            children[data[loc].suffix_link].push_back(loc);
            states_with_length[data[loc].len].push_back(loc);
        }

        ct_end_pos.resize(data.size());
        for (int len = SZ; len > 0; len--) {
            for (int loc : states_with_length[len]) {
                ct_end_pos[loc] += !is_clone[loc];
                ct_end_pos[data[loc].suffix_link] += ct_end_pos[loc];
            }
        }
    }

    int transition(int loc, int c) const {
        assert(loc != -1);
        return data[loc].transitions[c - MIN_CHAR];
    }

    template<typename I>
    int state_associated_with(I begin, I end) const {
        int loc = 0;
        for (auto iter = begin; iter != end && loc != -1; iter++)
            loc = transition(loc, *iter);
        return loc;
    }

    template<typename I>
    int first_occurence(I begin, I end) const {
        int loc = state_associated_with(begin, end);
        return loc == -1 ? -1 : data[loc].first_end_pos - distance(begin, end) + 1;
    }

    template<typename I>
    int count_occurences(I begin, I end) const {
        int loc = state_associated_with(begin, end);
        return loc == -1 ? 0 : ct_end_pos[loc];
    }
};

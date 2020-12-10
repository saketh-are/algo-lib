// {{{ misc/stable_counting_sort.cpp }}}

#include <array>
#include <vector>
#include <cassert>

template<int MIN_CHAR, int SIGMA>
struct suffix_automaton {
    struct state {
        int len = 0;
        int suffix_link = -1;
        int first_end_pos = -1;
        std::array<int, SIGMA> transitions;
        state() { transitions.fill(-1); }
    };

    int SZ, last;
    std::vector<state> data;
    std::vector<bool> is_clone;

    const state& operator [] (int loc) const { return data[loc]; }

    int create_state(state s = {}) {
        data.push_back(s);
        is_clone.push_back(s.len != 0);
        return int(data.size()) - 1;
    }

    suffix_automaton() : SZ(0) { last = create_state(); }

    template<typename InputIterator>
    suffix_automaton(InputIterator begin, InputIterator end) {
        initialize(begin, end);
    }

    template<typename InputIterator>
    void initialize(InputIterator begin, InputIterator end) {
        SZ = 0;
        data.clear();
        is_clone.clear();
        last = create_state();

        for (InputIterator iter = begin; iter != end; iter++)
            append(*iter);
        initialize_auxiliary_data();
    }

    void append(int c) {
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

    std::vector<int> reverse_length_order;
    std::vector<int> ct_end_pos;
    void initialize_auxiliary_data() {
        reverse_length_order = stable_counting_sort::permutation(
                int(data.size()), SZ + 1, [&](int loc) { return SZ - data[loc].len; });

        ct_end_pos.assign(data.size(), 0);
        for (int loc : reverse_length_order) {
            if (loc) {
                ct_end_pos[loc] += !is_clone[loc];
                ct_end_pos[data[loc].suffix_link] += ct_end_pos[loc];
            } else ct_end_pos[loc] = 0;
        }
    }

    int transition(int loc, int c) const {
        assert(loc != -1 && MIN_CHAR <= c && c < MIN_CHAR + SIGMA);
        return data[loc].transitions[c - MIN_CHAR];
    }

    template<typename InputIterator>
    int state_associated_with(InputIterator begin, InputIterator end) const {
        int loc = 0;
        for (auto iter = begin; iter != end && loc != -1; iter++)
            loc = transition(loc, *iter);
        return loc;
    }

    template<typename InputIterator>
    int first_occurrence(InputIterator begin, InputIterator end) const {
        int loc = state_associated_with(begin, end);
        return loc == -1 ? -1 : data[loc].first_end_pos - distance(begin, end) + 1;
    }

    template<typename InputIterator>
    int count_occurrences(InputIterator begin, InputIterator end) const {
        int loc = state_associated_with(begin, end);
        return loc == -1 ? 0 : ct_end_pos[loc];
    }
};

template<char MIN_CHAR, int SIGMA> struct trie {
    struct node {
        // link: contains trie links + failure links
        // suff: link to longest proper suffix that exists in the trie
        // dict: link to longest suffix that exists in the dictionary
        // wct: number of suffixes that are words in the dictionary
        // wid: index of this node's word in the dictionary
        array<int, SIGMA> link;
        int depth, suff, dict, wct, wid;
        node(int depth) : depth(depth), suff(0), dict(0), wct(0), wid(-1) {
            fill(all(link), 0);
        }
        int& operator[](char c) { return link[c - MIN_CHAR]; }
        int operator[](char c) const { return link[c - MIN_CHAR]; }
    };

    int W;
    vector<node> nodes;
    vpii tour;

    /*
     * Builds a trie over the given word set and calculates Aho-Corasick links.
     * Runs in O(sum of word lengths).
     */
    trie() {}
    const node& operator[](int i) const { return nodes[i]; }

    trie(auto begin, auto end) : W(0), nodes({ node(0) }) {
        for (auto it = begin; it != end; it++) {
            int loc = 0;
            for (char c : *it) {
                assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);
                if (!nodes[loc][c]) {
                    nodes[loc][c] = nodes.size();
                    nodes.push_back(nodes[loc].depth + 1);
                }
                loc = nodes[loc][c];
            }
            nodes[loc].dict = loc;
            assert(!nodes[loc].wct++);
            nodes[loc].wid = W++;
        }

        vvi ch(sz(nodes));
        for (queue<int> bfs({0}); !bfs.empty(); bfs.pop()) {
            int loc = bfs.front(), fail = nodes[loc].suff;
            if (loc) ch[fail].pb(loc);
            if (!nodes[loc].dict) nodes[loc].dict = nodes[fail].dict;
            nodes[loc].wct += nodes[fail].wct;

            for (char c = MIN_CHAR; c < MIN_CHAR + SIGMA; c++) {
                int& succ = nodes[loc][c];
                if (succ) {
                    nodes[succ].suff = loc ? nodes[fail][c] : 0;
                    bfs.push(succ);
                } else succ = nodes[fail][c];
            }
        }

        int e = 0;
        tour.resz(sz(nodes));
        auto dfs = [&](auto self, int loc) -> void {
            tour[loc].f = e++;
            for (int cloc : ch[loc]) self(self, cloc);
            tour[loc].s = e;
        };
        dfs(dfs, 0);
    }

    /*
     * Computes and returns the number of appearances of each word in the dictionary
     * as a substring of the given string.
     *
     * Runs in O(length of string to be searched + number of words in the dictionary).
     */
    vi matches(const string& text) const {
        vi res(W);
        priority_queue<pair<int, int>> found;

        int loc = 0;
        for (char c : text) {
            loc = nodes[loc].link[c - MIN_CHAR];
            int match = nodes[loc].dict;
            if (match) {
                if (!res[nodes[match].wid]++)
                    found.push({ nodes[match].depth, match });
            }
        }

        while (!found.empty()) {
            int loc = found.top().second; found.pop();
            int nxt = nodes[nodes[loc].suff].dict;
            if (nxt) {
                if (!res[nodes[nxt].wid]) found.push({ nodes[nxt].depth, nxt });
                res[nodes[nxt].id] += res[nodes[loc].id];
            }
        }
        return res;
    }

    /*
     * Computes and returns the total number of appearances of all words in the
     * dictionary as substrings of the given string.
     *
     * Runs in O(length of string to be searched).
     */
    ll search(const string& text) const {
        ll res = 0;
        int loc = 0;
        for (char c : text) {
            loc = nodes[loc].link[c - MIN_CHAR];
            res += nodes[loc].wct;
        }
        return res;
    }
};

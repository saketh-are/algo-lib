namespace aho_corasick {
    const int SIGMA = 2;
    const int TOTL = 1e7 + 100;

    struct node {
        int link[SIGMA];
        int suff, dict, patt;
        node() { 
            suff = 0, dict = 0, patt = -1;
            memset(link, 0, sizeof(link));
        }
        // link[]: contains trie links + failure links
        // suff: link to longest proper suffix that exists in the trie
        // dict: link to longest suffix that exists in the dictionary
        // patt: index of this node's word in the dictionary
    };

    int tail = 1;
    vector<node> trie(TOTL);
    vector<string> patterns;

    void add_pattern(string &s) {
        int loc = 0;
        for (char c : s) {
            int &nloc = trie[loc].link[c-'a'];
            if (!nloc) nloc = tail++;
            loc = nloc;
        }

        trie[loc].dict = loc;
        trie[loc].patt = patterns.size();
        patterns.push_back(s);
    }

    void calc_links() {
        queue<int> bfs({0});

        while (!bfs.empty()) {
            int loc = bfs.front(); bfs.pop();
            int fail = trie[loc].suff;

            if (!trie[loc].dict) 
                trie[loc].dict = trie[fail].dict;

            for (int c = 0; c < SIGMA; c++) {
                int &succ = trie[loc].link[c];
                if (succ) {
                    trie[succ].suff = loc ? trie[fail].link[c] : 0;
                    bfs.push(succ);
                } else succ = trie[fail].link[c];
            }
        }
    }

    void match(string &s, vector<bool> &matches) {
        int loc = 0;

        for (char c : s) {
            loc = trie[loc].link[c-'a'];

            for (int dm = trie[loc].dict; dm; dm = trie[trie[dm].suff].dict) {
                if (matches[trie[dm].patt]) break;
                matches[trie[dm].patt] = true;
            }
        }
    }
}


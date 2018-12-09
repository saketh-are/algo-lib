template<typename T> struct kmp {
    int P;
    vector<T> word;
    vi fail;

    int adv(int len, const T& nxt) const {
        while (len > 0 && word[len] != nxt)
            len = fail[len];
        return len + int(word[len] == nxt);
    }

    kmp(const vector<T>& word) : P(word.size()), word(word) {
        fail.resize(P + 1);
        for (int i = 2; i <= P; i++)
            fail[i] = adv(fail[i - 1], word[i - 1]);
    }

    vb find(const vector<T>& text) const {
        vb match(text.size());
        for (int i = 0, len = 0; i < text.size(); i++) {
            len = adv(len, text[i]);
            if (len == P) {
                match[i - len + 1] = true;
                len = fail[len];
            }
        }
        return match;
    }
};


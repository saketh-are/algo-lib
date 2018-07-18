template<typename T> struct kmp {
    int M;
    vector<T> needle;
    vector<int> succ;

    kmp(vector<T> _needle) {
        needle = _needle;
        M = needle.size();

        succ.resize(M + 1);
        succ[0] = -1, succ[1] = 0;

        int cur = 0;
        for (int i = 2; i <= M; ) {
            if (needle[i-1] == needle[cur]) succ[i++] = ++cur;
            else if (cur) cur = succ[cur];
            else succ[i++] = 0;
        }
    }

    vector<bool> find(vector<T> &haystack) {
        int N = haystack.size(), i = 0;
        vector<bool> res(N);

        for (int m = 0; m + i < N; ) {
            if (i < M && needle[i] == haystack[m + i]) {
                if (i == M - 1) res[m] = true;
                i++;
            } else if (succ[i] != -1) {
                m = m + i - succ[i];
                i = succ[i];
            } else {
                i = 0;
                m++;
            }
        }

        return res;
    }
};


namespace stable_counting_sort {
    template<typename T>
    T identity(const T& t) { return t; }

    template<typename T_extract_key = decltype(identity<int>)*>
    const vector<int>& permutation(int SZ, int MAX_KEY, const T_extract_key &extract_key = identity) {
        static vector<int> p;
        p.resize(SZ);

        if (SZ < 256) {
            iota(p.begin(), p.end(), 0);
            sort(p.begin(), p.end(), [&](int a, int b) {
                return extract_key(a) < extract_key(b);
            });
        } else {
            static vector<int> count;
            count.assign(MAX_KEY, 0);
            for (int i = 0; i < SZ; i++)
                count[extract_key(i)]++;
            for (int i = 0; i < MAX_KEY - 1; i++)
                count[i + 1] += count[i];
            for (int i = SZ - 1; i >= 0; i--)
                p[--count[extract_key(i)]] = i;
        }

        return p;
    }
};

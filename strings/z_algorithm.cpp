/*
 * Returns the vector z[i] = the greatest length l
 * such that s[i, i+l) is identical to s[0, l).
 */
template<typename T> vi z_algorithm(const vector<T>& s) {
    int N = s.size();
    vi z(N);
    z[0] = N;

    for (int i = 1, l, r = -1; i < N; i++) {
        z[i] = r > i ? min(r - i, z[i - l]) : 0;
        while (i + z[i] < N && s[i + z[i]] == s[z[i]])
            z[i]++;
        if (i + z[i] > r)
            l = i, r = i + z[i];
    }
    return z;
};


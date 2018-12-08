/*
 * Given a string of length N, returns a vector "pal" of 2N-1 elements
 * For even i, pal[i] = the length of the longest palindrome centered
 *                      at index i/2 of the input string
 * For odd i,  pal[i] = the length of the longest palindrome centered
 *                      between indices i/2 and i/2+1 of the input string
 */
template<typename T> vi manacher(const vector<T>& str) {
    vi pal(max(2 * int(str.size()) - 1, 0));

    auto match = [&str, &pal](int i, int l) {
        if (i-l < -1 || i+l > pal.size()) return false;
        return (i-l)&1 || str[(i-l)/2] == str[(i+l)/2];
    };

    for (int c = 0, end = -1, ref; c < pal.size(); c++) {
        if (end > c)
            pal[c] = min(pal[2 * ref - c], end - c);
        while (match(c, pal[c] + 1))
            pal[c]++;
        if (c + pal[c] > end) {
            ref = c;
            end = c + pal[c];
        }
    }
    return pal;
}


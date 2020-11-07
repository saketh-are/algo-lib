/* Given vector<int> v with sum T <= MAXT, computes the
 * set of subset sums of v in O((MAXT sqrt T) / machine_word_size).
 */
template<int MAXT>
bitset<MAXT + 1> subset_sum(const vector<int> &v) {
    int T = accumulate(v.begin(), v.end(), 0);
    assert(T <= MAXT);

    vector<int> appearances(T + 1);
    for (int val : v)
        appearances[val]++;

    bitset<MAXT + 1> result;
    result[0] = 1;

    for (int val = 1; val <= T; val++) {
        if (appearances[val] > 2) {
            int pairs = (appearances[val] - 1) / 2;
            appearances[2 * val] += pairs;
            appearances[val] -= 2 * pairs;
        }

        for (int rep = 0; rep < appearances[val]; rep++)
            result |= result << val;
    }

    return result;
}

#include <bitset>
#include <vector>
#include <cassert>

/* Accepts a collection of integers with sum at most MAX_SUM.
 * Computes its set of subset sums in O(MAX_SUM * sqrt(sum) / MACHINE_WORD_SIZE)
 */
template<int MAX_SUM, typename InputIterator>
std::bitset<MAX_SUM + 1> subset_sum(InputIterator first, InputIterator last) {
    int sum = accumulate(first, last, 0);
    assert(sum <= MAX_SUM);

    static std::vector<int> count(MAX_SUM + 1);

    for (InputIterator iter = first; iter != last; iter++)
        count[*iter]++;

    std::bitset<MAX_SUM + 1> result;
    result[0] = 1;

    for (int val = 1; val <= sum; val++) {
        if (count[val] > 2) {
            int pairs = (count[val] - 1) / 2;
            count[2 * val] += pairs;
            count[val] -= 2 * pairs;
        }

        while (count[val]) {
            result |= result << val;
            count[val]--;
        }
    }

    return result;
}

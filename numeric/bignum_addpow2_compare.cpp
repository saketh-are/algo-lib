// {{{ strings/polynomial_hash }}}

#include <vector>
#include <cassert>
#include <iostream>

struct bignum_addpow2_compare {
    using hash_t = polynomial_hash<modnum<int(1e9 + 7)>, 3, 2>;

    struct binary_string {
        hash_t hash;
        int ct_trailing_ones;
        int left, right;

        binary_string(bool bit) : hash(bit), ct_trailing_ones(bit), left(-1), right(-1) {}

        binary_string(hash_t _hash, int _ct_trailing_ones, int _left, int _right)
            : hash(_hash), ct_trailing_ones(_ct_trailing_ones), left(_left), right(_right) {}

        int length() const { return hash.N; }
        bool is_all_ones() const { return hash.N == ct_trailing_ones; }
    };

    std::vector<binary_string> nums = { binary_string(0), binary_string(1) };

    const int one = 1;
    std::vector<int> zero = { 0 };

    int concatenate(int x, int y) {
        nums.emplace_back(
            hash_t::concatenate(nums[x].hash, nums[y].hash),
            nums[y].ct_trailing_ones + (nums[y].is_all_ones() ? nums[x].ct_trailing_ones : 0),
            x,
            y
        );
        return int(nums.size()) - 1;
    }

    int get_zero_of_width(int bit_width) {
        int index = __builtin_ctz(bit_width);
        while (index >= int(zero.size()))
            zero.push_back(concatenate(zero.back(), zero.back()));
        return zero[index];
    }

    int carry_count(int x, int pow) const {
        const int len = nums[x].length();

        if (len <= pow)
            return 0;

        if (nums[x].is_all_ones())
            return len - pow;

        if (len == 1)
            return nums[x].ct_trailing_ones;

        int res = carry_count(nums[x].right, pow);

        if (pow + res >= len / 2)
            res += carry_count(nums[x].left, pow + res - len / 2);

        return res;
    }

    int invert_range(int x, int L, int R) {
        const int len = nums[x].length();
        assert(0 <= L && L < R && R <= len);

        if (len == 1)
            return x ^ 1;

        if (0 == L && R == len && nums[x].is_all_ones())
            return get_zero_of_width(len);

        int right = L < len / 2 ? invert_range(nums[x].right, L, std::min(R, len / 2)) : nums[x].right;
        int left  = R > len / 2 ? invert_range(nums[x].left, std::max(0, L - len / 2), R - len / 2) : nums[x].left;

        return concatenate(left, right);
    }

    int add_pow2(int x, int pow) {
        int carries = carry_count(x, pow);
        while (nums[x].length() <= pow + carries)
            x = concatenate(get_zero_of_width(nums[x].length()), x);
        return invert_range(x, pow, pow + carries + 1);
    }

    bool less_than(int x, int y) const {
        if (nums[x].length() != nums[y].length())
            return nums[x].length() < nums[y].length();

        if (nums[x].length() == 1)
            return nums[x].ct_trailing_ones < nums[y].ct_trailing_ones;

        if (nums[nums[x].left].hash == nums[nums[y].left].hash)
            return less_than(nums[x].right, nums[y].right);

        return less_than(nums[x].left, nums[y].left);
    }

    void print_bits(int x) const {
        if (nums[x].length() == 1) {
            std::cout << nums[x].ct_trailing_ones;
        } else {
            print_bits(nums[x].left);
            print_bits(nums[x].right);
        }
    }
};

#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>

// https://codeforces.com/blog/entry/62393

struct sp64_hash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

	static uint64_t random_address() { char *p = new char; delete p; return uint64_t(p); }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = splitmix64((random_address() | 1)
                * std::chrono::steady_clock::now().time_since_epoch().count());
        return splitmix64(x ^ FIXED_RANDOM);
    }
};

template<typename K>
struct sp64_pair_hash {
    size_t operator()(const std::pair<K, K>& x) const {
        static_assert(sizeof(K) <= 4, "type K has size exceeding 32 bits");
        static sp64_hash sp64;
        return sp64(((uint64_t) x.first << 32) ^ x.second);
    }
};

template<class K, class V>
using umap = __gnu_pbds::gp_hash_table<K, V, sp64_hash>;

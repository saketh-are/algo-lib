#include <vector>
#include <algorithm>

template<typename T, typename StaticCollection>
struct static_to_dynamic_transformation {
    int SZ;
    std::vector<T> elements;
    std::vector<StaticCollection> collections;

    static_to_dynamic_transformation() : SZ(0) {}

    void insert(T elt) {
        elements.push_back(elt);

        int index = __builtin_ctz(int(elements.size()));

        if (index >= int(collections.size()))
            collections.emplace_back();

        collections[index] = { elements.end() - (1 << index), elements.end() };

        std::fill(collections.begin(), collections.begin() + index, StaticCollection{});
    }

    template<typename Query, typename Result, typename CommutativeJoinResults>
    Result accumulate(Query query, Result init, CommutativeJoinResults join) const {
        for (const StaticCollection &c : collections)
            init = join(init, query(c));
        return init;
    }
};

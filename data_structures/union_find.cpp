#include <vector>
#include <numeric>
#include <iostream>

struct union_find {
    struct node {
        int parent, rank, size;
        node (int id = 0) : parent(id), rank(0), size(1) {}
    };

    mutable std::vector<node> data;

    union_find(int SZ = 0) : data(SZ) {
        iota(data.begin(), data.end(), 0);
    }

    // Returns the root of the component containing i
    int find(int i) const {
        if (i != data[i].parent)
            data[i].parent = find(data[i].parent);
        return data[i].parent;
    }

    bool is_root(int i) const {
        return i == find(i);
    }

    node& root_node(int i) const {
        return data[find(i)];
    }

    /* Unites the components containing a and b if they are different.
     * Returns a boolean indicating whether a and b were in different components.
     */
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;

        if (data[a].rank < data[b].rank)
            std::swap(a, b);

        data[b].parent = a;
        data[a].size += data[b].size;
        if (data[a].rank == data[b].rank)
            data[a].rank++;

        return true;
    }

    friend void pr(const union_find& u) {
        std::cout << "{";
        bool first = 1;
        for (int i = 0; i < int(u.data.size()); i++) {
            if (u.is_root(i)) {
                if (!first) std::cout << ", ";
                else first = 0;
                std::cout << "[ " << i << " | rank=" << u.data[i].rank
                    << " size=" << u.data[i].size << " ]";
            }
        }
        std::cout << "}";
    }
};

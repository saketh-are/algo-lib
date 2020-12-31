#include <vector>
#include <numeric>
#include <cassert>

// {{{ data_structures/splitmix64_hash_map }}}

struct union_find_rewindable {
    struct node {
        int parent, rank, size;
        node (int id = 0) : parent(id), rank(0), size(1) {}
    };

    struct modification {
        int parent, child;
        node previous_value;
    };

    std::vector<node> data;
    std::vector<modification> history;

    union_find_rewindable(int SZ = 0) : data(SZ) {
        iota(data.begin(), data.end(), 0);
    }

    // Returns the root of the component containing i
    int find(int i) const {
        while (i != data[i].parent)
            i = data[i].parent;
        return i;
    }

    bool is_root(int i) const {
        return i == find(i);
    }

    const node& root_node(int i) const {
        return data[find(i)];
    }

    /* Unites the components containing a and b if they are different.
     * Returns a boolean indicating whether a and b were in different components.
     */
    bool unite(int a, int b) {
        a = find(a), b = find(b);
        if(a == b) return false;

        if (data[a].rank < data[b].rank)
            std::swap(a, b);

        history.push_back({ a, b, data[a] });

        data[b].parent = a;
        data[a].size += data[b].size;
        if (data[a].rank == data[b].rank)
            data[a].rank++;

        return true;
    }

    int component_count() const {
        return int(data.size() - history.size());
    }

    int version() const {
        return int(history.size());
    }

    void rewind(int version_id) {
        assert(0 <= version_id && version_id <= int(history.size()));
        while (int(history.size()) > version_id) {
            auto [parent, child, previous_value] = history.back();

            data[parent] = previous_value;
            data[child].parent = child;

            history.pop_back();
        }
    }
};

/* Processes a sequence of events (u, v) toggling undirected edges
 * in a graph on vertex_count vertices. If an edge between u and v
 * is present, it's removed; otherwise, one is added.
 *
 * Returns f[x] = the number of components in the graph after
 * processing the first x events.
 */
std::vector<int> get_component_counts(int vertex_count,
        std::vector<std::pair<int, int>> edge_sequence) {
    const int E = int(edge_sequence.size());

    std::vector<int> matching_operation(E, E);
    __gnu_pbds::gp_hash_table<std::pair<int, int>, int,
        sp64_pair_hash<int>> inserted_by;

    for (int i = 0; i < E; i++) {
        auto it = inserted_by.find(edge_sequence[i]);
        if (it == inserted_by.end()) {
            inserted_by[edge_sequence[i]] = i;
        } else {
            matching_operation[it->second] = i;
            matching_operation[i] = it->second;
            inserted_by.erase(edge_sequence[i]);
        }
    }

    union_find_rewindable uf(vertex_count);

    std::vector<int> component_counts(E + 1);

    component_counts[0] = uf.component_count();

    auto solve = [&](auto& self, int first, int last) -> void {
        if (last - first == 1) {
            component_counts[last] = uf.component_count();
            if (matching_operation[first] > first) {
                auto [u, v] = edge_sequence[first];
                component_counts[last] -= uf.find(u) != uf.find(v);
            }
            return;
        }

        const int mid = first + (last - first) / 2;
        const int initial_state = uf.version();

        if (first < mid) {
            for (int i = mid; i < last; i++) {
                if (matching_operation[i] < first) {
                    auto [u, v] = edge_sequence[i];
                    uf.unite(u, v);
                }
            }
            self(self, first, mid);
            uf.rewind(initial_state);
        }

        if (mid < last) {
            for (int i = first; i < mid; i++) {
                if (matching_operation[i] >= last) {
                    auto [u, v] = edge_sequence[i];
                    uf.unite(u, v);
                }
            }
            self(self, mid, last);
            uf.rewind(initial_state);
        }
    };

    solve(solve, 0, E);

    return component_counts;
}

#include <vector>
#include <cassert>
#include <numeric>

struct union_find_bipartite {
    struct node {
        int parent, rank, size, status;
        bool parent_edge_parity;
        bool is_bipartite;
        int ct_nodes_on_root_side;

        node() {}

        node(int id) : parent(id), rank(0), size(1), status(-1),
                parent_edge_parity(0), is_bipartite(true), ct_nodes_on_root_side(1) {}

        int count_bipartitions() const {
            return !is_bipartite ? 0 : status == -1 ? 2 : 1;
        }

        int min_nodes_on_side_1() const {
            switch (status) {
                case  1: return ct_nodes_on_root_side;
                case  0: return size - ct_nodes_on_root_side;
                case -1: return std::min(ct_nodes_on_root_side, size - ct_nodes_on_root_side);
                default: assert(false);
            }
        }
    };

    int ct_components, ct_bipartite_components, degrees_of_freedom, min_nodes_on_side_1;
    mutable std::vector<node> data;

    union_find_bipartite(int N = 0) : ct_components(N), ct_bipartite_components(N),
            degrees_of_freedom(N), min_nodes_on_side_1(0), data(N) {
        iota(data.begin(), data.end(), 0);
    }

private:
    void subtract_component(int u) {
        ct_components--;
        if (data[u].is_bipartite) {
            ct_bipartite_components--;
            degrees_of_freedom -= data[u].status == -1;
            min_nodes_on_side_1 -= data[u].min_nodes_on_side_1();
        }
    }

    void add_component(int u) {
        ct_components++;
        if (data[u].is_bipartite) {
            ct_bipartite_components++;
            degrees_of_freedom += data[u].status == -1;
            min_nodes_on_side_1 += data[u].min_nodes_on_side_1();
        }
    }

public:
    int find(int u) const {
        if (u == data[u].parent) return u;
        find(data[u].parent);
        data[u].parent_edge_parity ^= data[data[u].parent].parent_edge_parity;
        return data[u].parent = data[data[u].parent].parent;
    }

    bool can_constrain_node_to_side(int u, bool side) const {
        find(u);
        side ^= data[u].parent_edge_parity;
        u = data[u].parent;

        return data[u].is_bipartite &&
            (data[u].status == -1 || data[u].status == side);
    }

    bool constrain_node_to_side(int u, bool side) {
        find(u);
        side ^= data[u].parent_edge_parity;
        u = data[u].parent;

        subtract_component(u);

        if (data[u].status == -1) {
            data[u].status = side;
        } else {
            data[u].is_bipartite &= data[u].status == side;
        }

        add_component(u);
        return data[u].is_bipartite;
    }

    bool can_add_constraint_on_nodes(int u, int v, bool edge_parity) const {
        find(u);
        edge_parity ^= data[u].parent_edge_parity;
        u = data[u].parent;

        find(v);
        edge_parity ^= data[v].parent_edge_parity;
        v = data[v].parent;

        return data[u].is_bipartite && data[v].is_bipartite &&
            (data[u].status == -1 || data[v].status == -1 || (data[u].status ^ data[v].status) == edge_parity);
    }

    struct result {
        bool added_connectivity;
        bool component_is_bipartite;
    };

    result unite(int u, int v, bool edge_parity) {
        find(u);
        edge_parity ^= data[u].parent_edge_parity;
        u = data[u].parent;

        find(v);
        edge_parity ^= data[v].parent_edge_parity;
        v = data[v].parent;

        if (u == v) {
            subtract_component(u);
            if (edge_parity)
                data[u].is_bipartite = false;
            add_component(u);
            return {false, data[u].is_bipartite};
        }

        if (data[u].rank < data[v].rank)
            std::swap(u, v);

        subtract_component(u);
        subtract_component(v);

        data[v].parent = u;

        data[v].parent_edge_parity = edge_parity;

        if (data[u].rank == data[v].rank)
            data[u].rank++;

        data[u].size += data[v].size;

        data[u].is_bipartite &= data[v].is_bipartite;

        if (edge_parity)
            data[u].ct_nodes_on_root_side += data[v].size - data[v].ct_nodes_on_root_side;
        else
            data[u].ct_nodes_on_root_side += data[v].ct_nodes_on_root_side;

        if (data[v].status != -1) {
            bool implied_u_status = data[v].status ^ edge_parity;
            if (data[u].status == -1)
                data[u].status = implied_u_status;
            else
                data[u].is_bipartite &= data[u].status == implied_u_status;
        }

        add_component(u);
        return {true, data[u].is_bipartite};
    }

    bool can_constrain_to_be_same      (int u, int v) const { return can_add_constraint_on_nodes(u, v, 0); }
    bool can_constrain_to_be_different (int u, int v) const { return can_add_constraint_on_nodes(u, v, 1); }

    result constrain_to_be_same        (int u, int v) { return unite(u, v, 0); }
    result constrain_to_be_different   (int u, int v) { return unite(u, v, 1); }
};

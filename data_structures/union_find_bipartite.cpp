struct union_find_bipartite {
    struct node {
        int parent;
        int size = 1, root_status = -1, same_as_root = 1;
        bool root_path_parity = 0, consistent = 1;
        node(int id) : parent(id) {}

        int choices() const {
            return !consistent ? 0 : root_status == -1 ? 2 : 1;
        }
        int minimum_size() const {
            if (root_status == 1) return same_as_root;
            else if (root_status == 0) return size - same_as_root;
            return min(same_as_root, size - same_as_root);
        }
    };
    vector<node> uf;
    int components, bipartite_components, rank, minimum_size;

    union_find_bipartite(int N = 0) : components(N), bipartite_components(N), rank(N), minimum_size(0) {
        for (int v = 0; v < N; v++) {
            uf.emplace_back(v);
        }
    }

    int rep(int u) {
        if (uf[u].parent == u) return u;
        int root = rep(uf[u].parent);
        uf[u].root_path_parity ^= uf[uf[u].parent].root_path_parity;
        return uf[u].parent = root;
    }

    void subtract_component(int u) {
        components--;
        if (uf[u].consistent) {
            bipartite_components--;
            rank -= uf[u].root_status == -1;
            minimum_size -= uf[u].minimum_size();
        }
    }

    void add_component(int u) {
        components++;
        if (uf[u].consistent) {
            bipartite_components++;
            rank += uf[u].root_status == -1;
            minimum_size += uf[u].minimum_size();
        }
    }

    bool set_status(int u, bool u_status) {
        int u_root = rep(u);
        subtract_component(u_root);

        const bool implied_root_status = u_status ^ uf[u].root_path_parity;
        if (uf[u_root].root_status == -1) {
            uf[u_root].root_status = implied_root_status;
        } else if (uf[u_root].root_status != implied_root_status) {
            uf[u_root].consistent = false;
        }

        add_component(u_root);
        return uf[u_root].consistent;
    }

    struct result { bool added_connectivity, is_consistent; };
    result unite(int u, int v, bool edge_parity) {
        int u_root = rep(u), v_root = rep(v);
        if (u_root == v_root) {
            subtract_component(u_root);
            if (edge_parity != (uf[u].root_path_parity ^ uf[v].root_path_parity)) {
                uf[u_root].consistent = false;
            }
            add_component(u_root);
            return {false, uf[u_root].consistent};
        }

        edge_parity ^= uf[u].root_path_parity ^ uf[v].root_path_parity;
        u = u_root;
        v = v_root;
        if (uf[u].size < uf[v].size)
            swap(u, v);

        subtract_component(u), subtract_component(v);

        uf[v].parent = u;
        uf[v].root_path_parity = edge_parity;
        uf[u].size += uf[v].size;
        uf[u].consistent &= uf[v].consistent;

        if (edge_parity)
            uf[u].same_as_root += uf[v].size - uf[v].same_as_root;
        else
            uf[u].same_as_root += uf[v].same_as_root;

        if (uf[v].root_status != -1) {
            bool implied_u_status = uf[v].root_status ^ edge_parity;
            uf[u].consistent &= (uf[u].root_status == -1 || uf[u].root_status == implied_u_status);
            uf[u].root_status = implied_u_status;
        }

        add_component(u);
        return {true, uf[u].consistent};
    }
    result constrain_to_be_same     (int u, int v) { return unite(u, v, 0); }
    result constrain_to_be_different(int u, int v) { return unite(u, v, 1); }

    // check consistency without recording constraint
    bool is_consistent(int u, bool u_status) {
        int u_root = rep(u);
        if (!uf[u_root].consistent) return false;
        if (uf[u_root].root_status == -1) return true;
        return u_status == (uf[u].root_path_parity ^ uf[u_root].root_status);
    }
    // check consistency of resulting component without recording constraint
    bool is_consistent(int u, int v, bool edge_parity) {
        int u_root = rep(u), v_root = rep(v);
        if (!uf[u_root].consistent || !uf[v_root].consistent) {
            return false;
        } else if (u_root == v_root) {
            return edge_parity == (uf[u].root_path_parity ^ uf[v].root_path_parity);
        } else if (uf[u_root].root_status == -1 || uf[v_root].root_status == -1) {
            return true;
        } else {
            return (uf[u].root_path_parity ^ uf[u_root].root_status
                  ^ uf[v].root_path_parity ^ uf[v_root].root_status) == edge_parity;
        }
    }
};

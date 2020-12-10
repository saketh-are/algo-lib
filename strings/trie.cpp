#include <array>
#include <vector>
#include <cassert>
#include <queue>

template<int MIN_CHAR, int SIGMA>
struct trie {
    struct node {
        int depth;
        std::array<int, SIGMA> child_links;

        int dict_index = -1;

        int suffix_link = 0;
        int dict_suffix_link = 0;
        int count_suffixes_in_dict = 0;

        node(int depth_) : depth(depth_) {
            child_links.fill(0);
        }
    };

    std::vector<node> data;
    std::vector<int> dictionary_word_links;

    int& child_link(int loc, int c) { return data[loc].child_links[c - MIN_CHAR]; }
    int child_link(int loc, int c) const { return data[loc].child_links[c - MIN_CHAR]; }

    int dict_proper_suffix_link(int loc) const {
        return data[data[loc].suffix_link].dict_suffix_link;
    }

    trie() {}

    template<typename InputIterator>
    trie(InputIterator first, InputIterator last) : data(1, node(0)) {
        for (InputIterator iter = first; iter != last; iter++)
            add_dictionary_word(iter->begin(), iter->end());
        build_suffix_link_tree();
    }

private:

    template<typename InputIterator>
    void add_dictionary_word(InputIterator first, InputIterator last) {
        int loc = 0;
        for (InputIterator iter = first; iter != last; iter++) {
            int c = *iter;
            assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);

            if (!child_link(loc, c)) {
                child_link(loc, c) = int(data.size());
                data.push_back(node(data[loc].depth + 1));
            }
            loc = child_link(loc, c);
        }
        if (data[loc].count_suffixes_in_dict == 0) {
            data[loc].dict_suffix_link = loc;
            data[loc].dict_index = int(dictionary_word_links.size());
        }
        data[loc].count_suffixes_in_dict++;
        dictionary_word_links.push_back(loc);
    }

    std::vector<std::vector<int>> children;
    std::vector<std::pair<int, int>> dfs_ranges;
    void build_suffix_link_tree() {
        children.resize(int(data.size()));

        std::queue<int> bfs;
        for (int child : data[0].child_links)
            if (child) bfs.push(child);
        for (; !bfs.empty(); bfs.pop()) {
            int loc = bfs.front();
            int parent = data[loc].suffix_link;

            children[parent].push_back(loc);
            if (data[loc].dict_suffix_link == 0)
                data[loc].dict_suffix_link = data[parent].dict_suffix_link;
            data[loc].count_suffixes_in_dict += data[parent].count_suffixes_in_dict;

            for (int c = MIN_CHAR; c < MIN_CHAR + SIGMA; c++) {
                int &trie_child = child_link(loc, c);
                if (trie_child) {
                    bfs.push(trie_child);
                    data[trie_child].suffix_link = child_link(parent, c);
                } else trie_child = child_link(parent, c);
            }
        }

        dfs_ranges.resize(data.size());

        int visited = 0;
        auto dfs = [&](auto self, int loc) -> void {
            dfs_ranges[loc].first = visited++;
            for (int child : children[loc])
                self(self, child);
            dfs_ranges[loc].second = visited;
        };
        dfs(dfs, 0);
    }

    template<typename V>
    void copy_results_for_duplicate_dictionary_entries(std::vector<V> &results) const {
        for (int dict_index = 0; dict_index < int(dictionary_word_links.size()); dict_index++) {
            int loc = dictionary_word_links[dict_index];
            if (data[loc].dict_index != dict_index)
                results[dict_index] = results[data[loc].dict_index];
        }
    }

public:

    /* Processes the given text and returns the number of matches of each dictionary word.
     * Linear in text length and the number of dictionary words.
     */
    template<typename InputIterator>
    std::vector<int> count_matches(InputIterator first, InputIterator last) const {
        std::vector<int> count(dictionary_word_links.size());
        std::vector<std::vector<int>> found_with_length;

        auto record_match = [&](int loc, int quantity) {
            int dict_index = data[loc].dict_index;
            if (dict_index == -1) return;

            if (count[dict_index] == 0) {
                if (data[loc].depth >= found_with_length.size())
                    found_with_length.resize(data[loc].depth + 1);
                found_with_length[data[loc].depth].push_back(loc);
            }
            count[dict_index] += quantity;
        };

        int loc = 0;
        for (InputIterator iter = first; iter != last; iter++) {
            int c = *iter;
            assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);

            loc = child_link(loc, c);
            record_match(data[loc].dict_suffix_link, 1);
        }

        for (int match_length = int(found_with_length.size()) - 1; match_length > 0; match_length--) {
            for (int match_loc : found_with_length[match_length])
                record_match(dict_proper_suffix_link(match_loc), count[data[match_loc].dict_index]);
        }

        copy_results_for_duplicate_dictionary_entries(count);
        return count;
    }

    /* Returns the starting index of every match of each dictionary word.
     * Linear in the text length, number of dictionary words, and total number of matches.
     */
    template<typename InputIterator>
    std::vector<std::vector<int>> indices_of_matches(InputIterator first, InputIterator last) const {
        std::vector<std::vector<int>> indices(int(dictionary_word_links.size()));

        int loc = 0;
        int index = 0;
        for (InputIterator iter = first; iter != last; iter++) {
            int c = *iter;
            index++;
            assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);

            loc = child_link(loc, c);
            for (int par = data[loc].dict_suffix_link; par != 0; par = dict_proper_suffix_link(par)) {
                indices[data[par].dict_index].push_back(index + 1 - data[par].depth);
            }
        }

        /* Notable fact: before duplication, the total number of matches is at most
         * (text length) * (number of distinct dictionary word lengths), which is
         * O(text length * sqrt(sum of dictionary word lengths)).
         */
        copy_results_for_duplicate_dictionary_entries(indices);
        return indices;
    }

    /* Returns the total number of matches over all dictionary words.
     * Duplicate dictionary entries each contribute to the total match count.
     * Linear in the text length.
     */
    template<typename InputIterator>
    int64_t count_total_matches(InputIterator first, InputIterator last) const {
        int64_t count = 0;

        int loc = 0;
        for (InputIterator iter = first; iter != last; iter++) {
            int c = *iter;
            assert(MIN_CHAR <= c && c < MIN_CHAR + SIGMA);

            loc = child_link(loc, c);
            count += data[loc].count_suffixes_in_dict;
        }

        return count;
    }
};

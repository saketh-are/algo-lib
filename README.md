[![CircleCI](https://circleci.com/gh/saketh-are/algo-lib.svg?style=shield)](https://circleci.com/gh/saketh-are/algo-lib)

## Contents
* [Usage](#usage)
  * [Passing function objects as arguments](#passing-function-objects-as-arguments)
  * [Passing iterators as arguments](#passing-iterators-as-arguments)
* [Data Structures](#data-structures)
  * [Union Find](#union-find)
  * [Union Find (Bipartite)](#union-find-bipartite)
  * [Union Find (Rewindable)](#union-find-rewindable)
  * [Segment Tree](#segment-tree)
  * [Segment Tree (Lazy Propagation)](README.md#segment-tree-lazy-propagation)
  * [Segment Tree (Persistent)](README.md#segment-tree-persistent)
  * [Segment Tree (Searchable)](README.md#segment-tree-searchable)
  * [Binary Indexed Tree](#binary-indexed-tree)
  * [Sparse Table](#sparse-table)
  * [Sqrt Decomposition (Range Update, Point Query)](#sqrt-decomposition-range-update-point-query)
  * [Line Container](#line-container)
  * [Line Container (Monotonic)](#line-container-monotonic)
  * [Static to Dynamic Transformation](#static-to-dynamic-transformation)
* [Graphs](#graphs)
  * [Tree](#tree)
  * [Lowest Common Ancestor](#lowest-common-ancestor)
  * [Heavy Path Decomposition](#heavy-path-decomposition)
  * [Dijkstra](#dijkstra)
  * [Two Sat](#two-sat)
* [Misc](#misc)
  * [Subset Sum](#subset-sum)
  * [Count Distinct In Range](#count-distinct-in-range)
* [Numeric](#numeric)
  * [ModNum](#modnum)
  * [Number Theoretic Transform](#number-theoretic-transform)
  * [Complex FFT](#complex-fft)
  * [Discrete Logarithm](#discrete-logarithm)
  * [Tonelli-Shanks](#tonelli-shanks)
  * [BigNum (Add Power of 2, Compare)](#bignum-add-power-of-2-compare)
* [Strings](#strings)
  * [Knuth-Morris-Pratt](#knuth-morris-pratt)
  * [Z Algorithm](#z-algorithm)
  * [Palindromes](#palindromes)
  * [Polynomial Hash](#polynomial-hash)
  * [Suffix Array](#suffix-array)
  * [Suffix Automaton](#suffix-automaton)
  * [Trie](#trie)
  * [Mutable String](#mutable-string)
  * [Mutable String (Bitset)](#mutable-string-bitset)
  
## Usage

All classes compile on C++ 17.

Some files require the contents of other files, denoted by a comment `// {{{ <filepath> }}}`. If you would like to generate buildable code from the usage examples, [a small script](.circleci/expand_dependencies.py) is provided to pull in dependencies. Run:

```bash
python2 expand_dependencies.py <input path> <output path> <path to repository root>
```

### Passing function objects as arguments

Some functions expect function objects as arguments, typically either:
* A binary operation used to accumulate stored values (e.g. [segment_tree](data_structures/segment_tree.cpp)'s constructor)
* A unary operation used to initialize a range of elements (e.g. [segment_tree](data_structures/segment_tree.cpp)'s assign)

You can pass a function pointer:

```c++
int add(int a, int b) {
    return a + b;
}

segment_tree<int, int(*)(int, int)> s(SZ, 0, add);
```

Or a lambda expression:
```c++
auto add = [](int a, int b) {
    return a + b;
};

segment_tree<int, decltype(add)> s(SZ, 0, add);
```

Or a struct with operator ():
```c++
struct adder {
    int operator()(int a, int b) {
        return a + b;
    }
};

segment_tree<int, adder> s(SZ, 0, adder{});
```

[functional.h](https://en.cppreference.com/w/cpp/header/functional) defines some useful function objects:
```c++
segment_tree<int, plus<int>> s(SZ, 0, plus<int>());
```

[Since C++17](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction) you can generally omit the template arguments: 
```c++
segment_tree s(SZ, 0, [](int a, int b) { return a + b; });
```

### Passing iterators as arguments

Some functions expect a pair of iterators `first` and `last` as arguments (e.g. [knuth_morris_pratt](strings/knuth_morris_pratt.cpp)'s constructor). 

* The range of elements operated on is always the semi-open interval `[first, last)`.

You'll typically pass either string iterators:

```c++
string s;
knuth_morris_pratt kmp(s.begin(), s.end());
```

Or vector iterators:
```c++
vector<int> s;
knuth_morris_pratt kmp(s.begin(), s.end());
```

## Data Structures

### [Union Find](data_structures/union_find.cpp)
Maintains a partition of the set `{0, 1, ..., N-1}` supporting:
* Retrieval of an identifier for the subset containing a specified element
* Replacement of two specified subsets with their union

Amortized runtime complexity is near constant for both operations.

 __Usage Examples:__
* [Roads not only in Berland](test/data_structures/union_find/roads_not_only_in_berland/roads_not_only_in_berland.cpp)

### [Union Find (Bipartite)](data_structures/union_find_bipartite.cpp)
Maintains information about bipartitions of the set `{0, 1, ..., N-1}` supporting:
 * Constrainment of two specified elements to appearing in opposite subsets
 * Constrainment of two specified elements to appearing in the same subset
 * Constrainment of a specified element to a specified subset
 * Counting the number of bipartitions satisfying all configured constraints
 * Computing the smallest size of a particular subset over all satisfying bipartitions

Amortized runtime complexity is near constant for all operations.

 __Usage Examples:__
* [Prefix Enlightenment](test/data_structures/union_find_bipartite/prefix_enlightenment/prefix_enlightenment.cpp)

### [Union Find (Rewindable)](data_structures/union_find_rewindable.cpp)
Maintains a partition of the set `{0, 1, ..., N-1}` supporting:
* Retrieval of an identifier for the subset containing a specified element in `O(log(N))`
* Replacement of two specified subsets with their union in `O(1)`
* Reversion of the most recent "union" operation in `O(1)`

Can process a sequence of `E` edge insertions and deletions in `O(E log(E) log(N))`.

 __Usage Examples:__
* [Connect and Disconnect](test/data_structures/union_find_bipartite/connect_and_disconnect/connect_and_disconnect.cpp)

### [Segment Tree](data_structures/segment_tree.cpp)
Maintains an array of `N` elements supporting:
* Modification of a specific element in `O(log(N))`
* Accumulation of a range of elements in `O(log(N))`

 __Usage Examples:__
* [Point Add Range Sum](test/data_structures/segment_tree/point_add_range_sum/point_add_range_sum.cpp)
* [Point Set Range Composite](test/data_structures/segment_tree/point_set_range_composite/point_set_range_composite.cpp)
* [Distinct Characters Queries](test/data_structures/segment_tree/distinct_characters_queries/distinct_characters_queries.cpp)

### [Segment Tree (Lazy Propagation)](data_structures/segment_tree_lazy.cpp)
Maintains an array of `N` elements supporting:
* Modification of a range of elements in `O(log(N))`
* Accumulation of a range of elements in `O(log(N))`

 __Usage Examples:__
* [Range Add Range Sum](test/data_structures/segment_tree_lazy/range_add_range_min/range_add_range_min.cpp)

### [Segment Tree (Persistent)](data_structures/segment_tree_persistent.cpp)
Maintains an array of `N` elements supporting:
* Modification of a specific element at a specified point in time in `O(log(N))`
* Accumulation of a range of elements as they were at a specified point in time in `O(log(N))`

Writes must be made with non-decreasing timestamps; there is no restriction on reads. Reads and writes can be interspersed in an arbitrary fashion.

Consumes `O(log(N))` additional memory on each write.

 __Usage Examples:__
* [K-Query](test/data_structures/segment_tree_persistent/k-query/k-query.cpp)

### [Segment Tree (Searchable)](data_structures/segment_tree_searchable.cpp)
Adds a binary search feature to the Segment Tree class which accepts an index `first` and a predicate `p`, returning the first index `last` such that `p(accumulate(first, last))` evaluates to true. Requires that the predicate is non-decreasing as `last` increases.

Rounds up internal size to the next power of 2. For a segment tree over `N` elements, search is `O(log(N))`.

 __Usage Examples:__
* [Deda](test/data_structures/segment_tree_searchable/deda/deda.cpp)

### [Binary Indexed Tree](data_structures/binary_indexed_tree.cpp)
Maintains an array of `N` elements supporting:
* Modification of a specific element in `O(log(N))`
* Accumulation of a prefix of elements in `O(log(N))`

Less general than a [Segment Tree](#segment-tree); offers better constant factor performance.

 __Usage Examples:__
* [Point Add Range Sum](test/data_structures/binary_indexed_tree/point_add_range_sum/point_add_range_sum.cpp)

### [Sparse Table](data_structures/sparse_table.cpp)
Accepts a static array of `N` elements and an idempotent binary operation `f`. Supports:
* Accumulation of a range of elements in `O(1)`

Precomputation is `O(N log(N))`. Typically used with `f = min<T>` or `f = max<T>`.

 __Usage Examples:__
* [Static RMQ](test/data_structures/sparse_table/static_rmq/static_rmq.cpp)

### [Sqrt Decomposition (Range Update, Point Query)](data_structures/sqrt_decomposition_point_query.cpp)
Maintains an array of `N` elements supporting:
* Modification of a range of elements in `O(sqrt(N))`
* Querying the value of a single element in `O(1)`

 __Usage Examples:__
* [Duff is Mad](test/data_structures/sqrt_decomposition_point_query/duff_is_mad/duff_is_mad.cpp)

### [Line Container](data_structures/line_container.cpp)
Maintains a collection of lines. Supports:
 * Insertion of a line `f(x) = a * x + b`
 * Querying for the maximum value among all inserted lines at a specified value of `x`

Runtime complexity for both operations is `O(log(container size))`.

 __Usage Examples:__
* [Escape Through Leaf](test/data_structures/line_container/escape_through_leaf/escape_through_leaf.cpp)

### [Line Container (Monotonic)](data_structures/line_container_monotonic.cpp)
Maintains a collection of lines. Supports:
 * Insertion of a line `f(x) = a * x + b`, where `a` is non-decreasing across all insertions
 * Querying for the maximum value among all inserted lines at a specified value of `x`

Runtime complexity for insertion is amortized `O(1)`. Runtime complexity for arbitrary queries is `O(log(container size))`. If `get_maximum_monotonic` is used, total runtime complexity over a sequence of queries made with non-decreasing `x` is linear. 

 __Usage Examples:__
* [The Fair Nut and Rectangles](test/data_structures/line_container_monotonic/the_fair_nut_and_rectangles/the_fair_nut_and_rectangles.cpp)
* [Cats Transport](test/data_structures/line_container_monotonic/cats_transport/cats_transport.cpp)

### [Static to Dynamic Transformation](data_structures/static_to_dynamic_transformation.cpp)
Adds the ability to insert new elements into a static data structure. Requires that queries to the data structure are decomposable; for any pair of disjoint sets of elements, the answer to a query over their union must be computable from answers to the query over the individual sets.

Over a sequence of `N` insertions, the K-th most recently inserted element is involved in `log(K)` constructions of the static data structure. A query made after `N` elements have been inserted is decomposed into `log(N)` queries on disjoint subsets of the inserted elements.

 __Usage Examples:__
* [String Set Queries](test/data_structures/static_to_dynamic_transformation/string_set_queries/string_set_queries.cpp)

## Graphs

### [Tree](graphs/tree.cpp)
Computes and stores basic properties of a rooted tree on `V` nodes:
* The parent, depth, and subtree size of each node
* The neighbor list for each node in non-increasing order of subtree size
* A preorder traversal of the tree

Precomputation is `O(V log(V))` to compute sorted neighbor lists and `O(V)` otherwise.

__Usage Examples:__
* [Weighted Tree Diameter](test/graphs/tree/weighted_tree_diameter/weighted_tree_diameter.cpp)

### [Lowest Common Ancestor](graphs/lowest_common_ancestor.cpp)
Computes and stores the euler tour representation of a tree on `V` nodes:
* Supports queries for the lowest common ancestor of two nodes in `O(1)`
* Supports queries for the distance between two nodes in `O(1)`
* Supports queries for whether the simple path between two nodes visits a third node in `O(1)`
* Supports queries for whether a specified node is an ancestor of another in `O(1)`
* Supports queries for the first step on the simple path between two nodes in `O(1)`

Precomputation is `O(V log(V))`.

__Usage Examples:__
* [Lowest Common Ancestor](test/graphs/lowest_common_ancestor/lowest_common_ancestor/lowest_common_ancestor.cpp)

### [Heavy Path Decomposition](graphs/heavy_path_decomposition.cpp)
Given a rooted tree on `V` nodes, computes a permutation `p` of its vertices such that:
* Any simple path in the tree can be decomposed into `log(V)` contiguous oriented intervals in `p`
* Any subtree's vertex set occurs as a single contiguous interval in `p`

Accepts an instance of the [Tree](#tree) class. Additional precomputation is `O(V)`.

__Usage Examples:__
* [Vertex Add Path Sum](test/graphs/heavy_path_decomposition/vertex_add_path_sum/vertex_add_path_sum.cpp)

### [Dijkstra](graphs/dijkstra.cpp)
An implementation of Dijkstra's Algorithm for computing shortest path trees.

Runs in `O(V + E log(E))` on a graph with `V` vertices and `E` edges.

__Usage Examples:__
* [Shortest Path](test/graphs/dijkstra/shortest_path/shortest_path.cpp)

### [Two Sat](graphs/two_sat.cpp)
Implication graph for boolean two-satisfiability. Constructs a satisfying assignment or detects that none exist. Runtime is linear in the size of the implication graph.

Classifies each variable as true in all satisfying assignments, false in all satisfying assignments, or neither. Runtime is `O(N * N / MACHINE_WORD_SIZE)`.

 __Usage Examples:__
* [Two Sat](test/graphs/two_sat/two_sat/two_sat.cpp)

## Misc

### [Subset Sum](misc/subset_sum.cpp)
Given a set of integers with sum `T`, computes its set of subset sums in `O(T sqrt(T) / MACHINE_WORD_SIZE)`.

__Usage Examples:__
 * [PolandBall and Gifts](test/misc/subset_sum/polandball_and_gifts/polandball_and_gifts.cpp)

### [Count Distinct In Range](misc/count_distinct_in_range.cpp)
Processes a sequence of comparable elements and a parameter `copies_allowed`. Supports queries for the number of elements in a specified range of the sequence if only the first `copies_allowed` appearances of each distinct value are counted.

__Usage Examples:__
 * [Range Set Query](test/misc/count_distinct_in_range/range_set_query/range_set_query.cpp)
 * [Army Creation](test/misc/count_distinct_in_range/army_creation/army_creation.cpp)

## Numeric

### [ModNum](numeric/modnum.cpp)
Performs modular arithmetic:
 * Implements basic operators (`+`, `-`, `*`, `/`)
 * Computes inverses in `O(log(MOD))`
 * Computes discrete logarithms in `O(sqrt(MOD))`
 * Caches roots of unity
 * Caches factorials and inverse factorials to compute:
   * Binomial coefficients in `O(1)`
   * Inverses in `O(1)`

`MOD` is configured as a template parameter. If the modulus isn't known at compile-time, move `MOD` inside the struct as a static member.

 __Usage Examples:__
 * [Exponentiation](test/numeric/modnum/exponentiation/exponentiation.cpp)
 
 ### [Number Theoretic Transform](numeric/ntt.cpp)
 
 Convolves polynomials over the field of integers modulo `P`.
 
 Requires that `P` is prime. Let `D` be the smallest power of 2 exceeding the degree of the returned polynomial. `D` must divide `P-1`.
 
 The runtime complexity is `O(D log(D))`. 
  
 __Usage Examples:__
 * [Convolution Mod 998244353](test/numeric/ntt/convolution_mod_998244353/convolution_mod_998244353.cpp)
 
 ### [Complex FFT](numeric/complex_fft.cpp)
 
 Convolves polynomials over the field of doubles.
 
 Provides a routine for convolution of polynomials over the integers modulo `P` _without_ [Number Theoretic Transform](#number-theoretic-transform)'s constraint that `D` divides `P - 1`. This routine seems to be safe for `N,M <= 150,000` but isn't precise enough for some adversarial cases around `N = M = 180,000`.
 
 __Usage Examples:__
 * [Convolution Mod 1000000007](test/numeric/complex_fft/convolution_mod_1000000007/convolution_mod_1000000007.cpp)

 ### [Discrete Logarithm](numeric/discrete_logarithm.cpp)
 
 Given integers `x`, `y`, and `MOD`, computes and returns the smallest non-negative integer `K` such that `x^K === y (mod MOD)`. Returns `-1` if no such integer exists. Runs in `O(sqrt(MOD))`.
 
 __Usage Examples:__
 * [Discrete Logarithm](test/numeric/discrete_logarithm/discrete_logarithm/discrete_logarithm.cpp)
 
 ### [Tonelli-Shanks](numeric/tonelli-shanks.cpp)
 
 Given integers `n` and `MOD`, computes and returns an integer `r` such that `r * r === n (mod MOD)`. Returns `-1` if no such integer exists.
 
 Requires that `MOD` is a prime power. If `MOD = 2^S * Q` with `Q` odd, runs in `O(S log(S) / log(log(S)))`.
 
 __Usage Examples:__
 * [Sqrt Mod](test/numeric/tonelli-shanks/sqrt_mod/sqrt_mod.cpp)
 
 ### [BigNum (Add Power of 2, Compare)](numeric/bignum_addpow2_compare.cpp)
 
Large integer representation. Starts with a representation of `0` and supports:
 * Representation of the sum of a represented integer with an arbitrary power of 2
 * Comparison of two represented integers
 
 Runtime complexity for both operations is logarithmic in the number of digits of precision. Constructing a sum also consumes additional memory logarithmic in the number of digits of precision.
 
 __Usage Examples:__
 * [The Classic Problem](test/numeric/bignum_addpow2_compare/the_classic_problem/the_classic_problem.cpp)
 
## Strings

### [Knuth-Morris-Pratt](strings/knuth_morris_pratt.cpp)
Given a string `S`, computes for each prefix the longest proper suffix which is also a prefix of `S`. Supports:
 * Characterizing occurrences (and partial occurrences) of `S` in a specified pattern `T` in `O(|T|)`
 
Precomputation is `O(|S|)`.

 __Usage Examples:__
 * [Pattern Find](test/strings/knuth_morris_pratt/pattern_find/pattern_find.cpp)

### [Z Algorithm](strings/z_algorithm.cpp)
Given a string `S`, computes the length of the longest common prefix between `S` and each of its suffixes.

Runs in `O(|S|)`.

 __Usage Examples:__
 * [Z Algorithm](test/strings/z_algorithm/z_algorithm/z_algorithm.cpp)

### [Palindromes](strings/palindromes.cpp)
Given a string `S`:
 * Computes the length of the longest palindrome centered at each character
 * Computes the length of the longest palindrome centered between each pair of adjacent characters
 * Supports queries for the length of the longest palindrome within a specified substring of `S` in `O(log(|S|))`

Precomputation is `O(|S|)`. Additional precomputation for substring queries is `O(|S| log(|S|))`.

 __Usage Examples:__
 * [Enumerate Palindromes](test/strings/palindromes/enumerate_palindromes/enumerate_palindromes.cpp)

### [Polynomial Hash](strings/polynomial_hash.cpp)
A rolling hash for probabilistic string equality checks. Supports:
 * Hashing a character in `O(1)`
 * Concatenating two hashes in `O(1)`
 * Computing hashes for all prefixes of a given string `S` in `O(|S|)`
 * Given prefix hashes for a string `S`, extracting the hash for an arbitrary substring of `S` in `O(1)`

Increasing parameter _EvaluationPoints_ lowers the probability of two distinct strings producing the same hash.

 __Usage Examples:__
 * [Good Substrings](test/strings/polynomial_hash/good_substrings/good_substrings.cpp)

### [Suffix Array](strings/suffix_array.cpp)
Given a string `S`:
  * Computes the lexicographic ordering of its suffixes
  * Computes the length of longest common prefix between each pair of adjacent suffixes in the lexicographic ordering
  * Supports queries for the longest common prefix between an arbitrary pair of suffixes
  * Supports queries for the range of ranks at which a specified substring occurs
  * Supports queries for the first occurrence of a specified substring

Precomputation is `O(|S| log(|S|))`.

__Usage Examples:__
* [Suffix Array](test/strings/suffix_array/suffix_array/suffix_array.cpp)
* [Count Distinct Substrings](test/strings/suffix_array/count_distinct_substrings/count_distinct_substrings.cpp)

### [Suffix Automaton](strings/suffix_automaton.cpp)
Given a string `S`, computes the minimal finite automaton accepting all suffixes of `S`.
* Supports queries for the first occurrence in `S` of a specified pattern `T` in `O(|T|)`
* Supports queries for the number of occurrences in `S` of a specified pattern `T` in `O(|T|)`

Precomputation is `O(|S|)`.

__Usage Examples:__
* [Count Distinct Substrings](test/strings/suffix_automaton/count_distinct_substrings/count_distinct_substrings.cpp)

### [Trie](strings/trie.cpp)
Given a collection of strings `S`, supports:
* Computing the total number of occurrences over all strings in `S` within a specified pattern `T` in `O(|T|)`
* Computing the number of occurrences of each string in `S` within a specified pattern `T` in `O(|S| + |T|)`
* Finding the indices of all occurrences of each string in `S` within a specified pattern `T` in `O(|T| + |S| + total number of occurrences)`

Precomputation is linear in the sum of the lengths of the strings in `S`.

__Usage Examples:__
* [Minimum Length Substring With K Occurrences](test/strings/trie/minimum_length_substring_with_k_occurrences/minimum_length_substring_with_k_occurrences.cpp)

### [Mutable String](strings/mutable_string.cpp)
Maintains a string over a finite alphabet, supporting:
* Modification of a specified character in `O(sqrt(|S|))`
* Queries for the number of appearances of a specified pattern `T` in a specified substring of `S` in `O(|T|sqrt(|S|))`

__Usage Examples:__
* [Count Matches in Mutable Text](test/strings/mutable_string/count_matches_in_mutable_text/count_matches_in_mutable_text.cpp)

### [Mutable String (Bitset)](strings/mutable_string_bitset.cpp)
Maintains a string over a finite alphabet, supporting:
* Modification of a specified character in `O(1)`
* Queries for the number of appearances of a specified pattern `T` in a specified substring of `S` in `O(|T| * |S| / MACHINE_WORD_SIZE)`

__Usage Examples:__
* [Count Matches in Mutable Text](test/strings/mutable_string_bitset/count_matches_in_mutable_text/count_matches_in_mutable_text.cpp)

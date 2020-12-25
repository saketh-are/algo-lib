[![CircleCI](https://circleci.com/gh/saketh-are/algo-lib.svg?style=shield)](https://circleci.com/gh/saketh-are/algo-lib)

## Contents
* [Usage](https://github.com/saketh-are/algo-lib#Usage)
  * [Passing function objects as arguments](https://github.com/saketh-are/algo-lib#passing-function-objects-as-arguments)
  * [Passing iterators as arguments](https://github.com/saketh-are/algo-lib#passing-iterators-as-arguments)
* [Data Structures](https://github.com/saketh-are/algo-lib#data-structures)
  * [Segment Tree](https://github.com/saketh-are/algo-lib#segment-tree)
  * [Segment Tree (Lazy Propagation)](https://github.com/saketh-are/algo-lib/blob/master/README.md#segment-tree-lazy-propagation)
  * [Binary Indexed Tree](https://github.com/saketh-are/algo-lib#binary-indexed-tree)
  * [Sparse Table](https://github.com/saketh-are/algo-lib#sparse-table)
* [Graphs](https://github.com/saketh-are/algo-lib#graphs)
  * [Tree](https://github.com/saketh-are/algo-lib#tree)
  * [Lowest Common Ancestor](https://github.com/saketh-are/algo-lib#lowest-common-ancestor)
  * [Heavy Path Decomposition](https://github.com/saketh-are/algo-lib#heavy-path-decomposition)
  * [Dijkstra](https://github.com/saketh-are/algo-lib#dijkstra)
  * [Two Sat](https://github.com/saketh-are/algo-lib#two-sat)
* [Misc](https://github.com/saketh-are/algo-lib#misc)
  * [Subset Sum](https://github.com/saketh-are/algo-lib#subset-sum)
  * [Count Distinct In Range](https://github.com/saketh-are/algo-lib#count-distinct-in-range)
* [Numeric](https://github.com/saketh-are/algo-lib#numeric)
  * [ModNum](https://github.com/saketh-are/algo-lib#modnum)
  * [Number Theoretic Transform](https://github.com/saketh-are/algo-lib#number-theoretic-transform)
  * [Complex FFT](https://github.com/saketh-are/algo-lib#complex-fft)
  * [Discrete Logarithm](https://github.com/saketh-are/algo-lib#discrete-logarithm)
  * [Tonelli-Shanks](https://github.com/saketh-are/algo-lib#tonelli-shanks)
* [Strings](https://github.com/saketh-are/algo-lib#strings)
  * [Knuth-Morris-Pratt](https://github.com/saketh-are/algo-lib#knuth-morris-pratt)
  * [Z Algorithm](https://github.com/saketh-are/algo-lib#z-algorithm)
  * [Palindromes](https://github.com/saketh-are/algo-lib#palindromes)
  * [Polynomial Hash](https://github.com/saketh-are/algo-lib#polynomial-hash)
  * [Suffix Array](https://github.com/saketh-are/algo-lib#suffix-array)
  * [Suffix Automaton](https://github.com/saketh-are/algo-lib#suffix-automaton)
  * [Trie](https://github.com/saketh-are/algo-lib#trie)
  * [Mutable String](https://github.com/saketh-are/algo-lib#mutable-string)
  * [Mutable String (Bitset)](https://github.com/saketh-are/algo-lib#mutable-string-bitset)
  
## Usage

All classes compile on C++ 17.

Some files require the contents of other files, denoted by a comment `// {{{ <filepath> }}}`. If you would like to generate buildable code from the usage examples, [a small script](https://github.com/saketh-are/algo-lib/blob/master/.circleci/expand_dependencies.py) is provided to pull in dependencies. Run:

```bash
python2 expand_dependencies.py <input path> <output path> <path to repository root>
```

### Passing function objects as arguments

Some functions expect function objects as arguments, typically either:
* A binary operation used to accumulate stored values (e.g. [segment_tree](https://github.com/saketh-are/algo-lib/blob/master/data_structures/segment_tree.cpp) 's constructor)
* A unary operation used to initialize a range of elements (e.g. [segment_tree](https://github.com/saketh-are/algo-lib/blob/master/data_structures/segment_tree.cpp)'s assign)

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

Some functions expect a pair of iterators `first` and `last` as arguments (e.g. [knuth_morris_pratt](https://github.com/saketh-are/algo-lib/blob/master/strings/knuth_morris_pratt.cpp)'s constructor). 

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

### [Segment Tree](https://github.com/saketh-are/algo-lib/blob/master/data_structures/segment_tree.cpp)
Maintains an array of `N` elements supporting:
* Modification of a specific element in `O(log(N))`
* Accumulation of a range of elements in `O(log(N))`

 __Usage Examples:__
* [Point Add Range Sum](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/segment_tree/point_add_range_sum/point_add_range_sum.cpp)
* [Point Set Range Composite](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/segment_tree/point_set_range_composite/point_set_range_composite.cpp)
* [Distinct Characters Queries](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/segment_tree/distinct_characters_queries/distinct_characters_queries.cpp)

### [Segment Tree (Lazy Propagation)](https://github.com/saketh-are/algo-lib/blob/master/data_structures/segment_tree_lazy.cpp)
Maintains an array of `N` elements supporting:
* Modification of a range of elements in `O(log(N))`
* Accumulation of a range of elements in `O(log(N))`

 __Usage Examples:__
* [Range Add Range Sum](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/segment_tree_lazy/range_add_range_min/range_add_range_min.cpp)

### [Binary Indexed Tree](https://github.com/saketh-are/algo-lib/blob/master/data_structures/binary_indexed_tree.cpp)
Maintains an array of `N` elements supporting:
* Modification of a specific element in `O(log(N))`
* Accumulation of a prefix of elements in `O(log(N))`

Less general than a [Segment Tree](https://github.com/saketh-are/algo-lib#segment-tree); offers better constant factor performance.

 __Usage Examples:__
* [Point Add Range Sum](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/binary_indexed_tree/point_add_range_sum/point_add_range_sum.cpp)

### [Sparse Table](https://github.com/saketh-are/algo-lib/blob/master/data_structures/sparse_table.cpp)
Accepts a static array of `N` elements and an idempotent binary operation `f`. Supports:
* Accumulation of a range of elements in `O(1)`

Precomputation is `O(N log(N)`. Typically used with `f = min<T>` or `f = max<T>`.

 __Usage Examples:__
* [Static RMQ](https://github.com/saketh-are/algo-lib/blob/master/test/data_structures/sparse_table/static_rmq/static_rmq.cpp)

## Graphs

### [Tree](https://github.com/saketh-are/algo-lib/blob/master/graphs/tree.cpp)
Computes and stores basic properties of a rooted tree on `V` nodes:
* The parent, depth, and subtree size of each node
* The neighbor list for each node in non-increasing order of subtree size
* A preorder traversal of the tree

Precomputation is `O(V log(V))` to compute sorted neighbor lists and `O(V)` otherwise.

__Usage Examples:__
* [Weighted Tree Diameter](https://github.com/saketh-are/algo-lib/blob/master/test/graphs/tree/weighted_tree_diameter/weighted_tree_diameter.cpp)

### [Lowest Common Ancestor](https://github.com/saketh-are/algo-lib/blob/master/graphs/lowest_common_ancestor.cpp)
Computes and stores the euler tour representation of a tree on `V` nodes:
* Supports queries for the lowest common ancestor of two nodes in `O(1)`
* Supports queries for the distance between two nodes in `O(1)`
* Supports queries for whether the simple path between two nodes visits a third node in `O(1)`
* Supports queries for whether a specified node is an ancestor of another in `O(1)`
* Supports queries for the first step on the simple path between two nodes in `O(1)`

Precomputation is `O(V log(V))`.

__Usage Examples:__
* [Lowest Common Ancestor](https://github.com/saketh-are/algo-lib/blob/master/test/graphs/lowest_common_ancestor/lowest_common_ancestor/lowest_common_ancestor.cpp)

### [Heavy Path Decomposition](https://github.com/saketh-are/algo-lib/blob/master/graphs/heavy_path_decomposition.cpp)
Given a rooted tree on `V` nodes, computes a permutation `p` of its vertices such that:
* Any simple path in the tree can be decomposed into `log(V)` contiguous oriented intervals in `p`
* Any subtree's vertex set occurs as a single contiguous interval in `p`

Accepts an instance of the [Tree](https://github.com/saketh-are/algo-lib#tree) class. Additional precomputation is `O(V)`.

__Usage Examples:__
* [Vertex Add Path Sum](https://github.com/saketh-are/algo-lib/blob/master/test/graphs/heavy_path_decomposition/vertex_add_path_sum/vertex_add_path_sum.cpp)

### [Dijkstra](https://github.com/saketh-are/algo-lib/blob/master/graphs/tree.cpp)
An implementation of Dijkstra's Algorithm for computing shortest path trees.

Runs in `O(V + E log(E))` on a graph with `V` vertices and `E` edges.

__Usage Examples:__
* [Shortest Path](https://github.com/saketh-are/algo-lib/blob/master/test/graphs/dijkstra/shortest_path/shortest_path.cpp)

### [Two Sat](https://github.com/saketh-are/algo-lib/blob/master/graphs/two_sat.cpp)
Implication graph for boolean two-satisfiability. Constructs a satisfying assignment or detects that none exist. Runtime is linear in the size of the implication graph.

Classifies each variable as true in all satisfying assignments, false in all satisfying assignments, or neither. Runtime is `O(N * N / MACHINE_WORD_SIZE)`.

 __Usage Examples:__
* [Two Sat](https://github.com/saketh-are/algo-lib/blob/master/test/graphs/two_sat/two_sat/two_sat.cpp)

## Misc

### [Subset Sum](https://github.com/saketh-are/algo-lib/blob/master/misc/subset_sum.cpp)
Given a set of integers with sum `T`, computes its set of subset sums in `O(T sqrt(T) / MACHINE_WORD_SIZE)`.

__Usage Examples:__
 * [PolandBall and Gifts](https://github.com/saketh-are/algo-lib/blob/master/test/misc/subset_sum/polandball_and_gifts/polandball_and_gifts.cpp)

### [Count Distinct In Range](https://github.com/saketh-are/algo-lib/blob/master/misc/count_distinct_in_range.cpp)
Processes a sequence of comparable elements and a parameter `copies_allowed`. Supports queries for the number of elements in a specified range of the sequence if only the first `copies_allowed` appearances of each distinct value are counted.

__Usage Examples:__
 * [Range Set Query](https://github.com/saketh-are/algo-lib/blob/master/test/misc/count_distinct_in_range/range_set_query/range_set_query.cpp)

## Numeric

### [ModNum](https://github.com/saketh-are/algo-lib/blob/master/numeric/modnum.cpp)
Performs modular arithmetic:
 * Implements basic operators (`+`, `-`, `*`, `/`)
 * Computes inverses in `O(log(MOD))`
 * Computes discrete logarithms in `O(sqrt(MOD))`
 * Caches roots of unity
 * Caches factorials and inverse factorials to compute:
   * Binomial coefficients in `O(1)`
   * Inverses in `O(1)`

`MOD` is configured as a template parameter. If the modulus isn't know at compile-time, move `MOD` inside the struct as a static member.

 __Usage Examples:__
 * [Exponentiation](https://github.com/saketh-are/algo-lib/blob/master/test/numeric/modnum/exponentiation/exponentiation.cpp)
 
 ### [Number Theoretic Transform](https://github.com/saketh-are/algo-lib/blob/master/numeric/ntt.cpp)
 
 Convolves polynomials over the field of integers modulo `P`.
 
 Requires that `P` is prime. Let `D` be the smallest power of 2 exceeding the degree of the returned polynomial. `D` must divide `P-1`.
 
 The runtime complexity is `O(D log(D))`. 
  
 __Usage Examples:__
 * [Convolution Mod 998244353](https://github.com/saketh-are/algo-lib/blob/master/test/numeric/ntt/convolution_mod_998244353/convolution_mod_998244353.cpp)
 
 ### [Complex FFT](https://github.com/saketh-are/algo-lib/blob/master/numeric/complex_fft.cpp)
 
 Convolves polynomials over the field of doubles.
 
 Provides a routine for convolution of polynomials over the integers modulo `P` _without_ [Number Theoretic Transform](https://github.com/saketh-are/algo-lib/blob/master/numeric/ntt.cpp)'s constraint that `D` divides `P - 1`. This routine seems to be safe for `N,M <= 150,000` but isn't precise enough for some adversarial cases around `N = M = 180,000`.
 
 __Usage Examples:__
 * [Convolution Mod 1000000007](https://github.com/saketh-are/algo-lib/blob/master/test/numeric/complex_fft/convolution_mod_1000000007/convolution_mod_1000000007.cpp)

 ### [Discrete Logarithm](https://github.com/saketh-are/algo-lib/blob/master/numeric/discrete_logarithm.cpp)
 
 Given integers `x`, `y`, and `MOD`, computes and returns the smallest non-negative integer `K` such that `x^K === y (mod MOD)`. Returns `-1` if no such integer exists. Runs in `O(sqrt(MOD))`.
 
 __Usage Examples:__
 * [Discrete Logarithm](https://github.com/saketh-are/algo-lib/blob/master/test/numeric/discrete_logarithm/discrete_logarithm/discrete_logarithm.cpp)
 
 ### [Tonelli-Shanks](https://github.com/saketh-are/algo-lib/blob/master/numeric/tonelli-shanks.cpp)
 
 Given integers `n` and `MOD`, computes and returns an integer `r` such that `r * r === n (mod MOD)`. Returns `-1` if no such integer exists.
 
 Requires that `MOD` is a prime power. If `MOD = 2^S * Q` with `Q` odd, runs in `O(S log(S) / log(log(S)))`.
 
 __Usage Examples:__
 * [Sqrt Mod](https://github.com/saketh-are/algo-lib/blob/master/test/numeric/tonelli-shanks/sqrt_mod/sqrt_mod.cpp)
 
## Strings

### [Knuth-Morris-Pratt](https://github.com/saketh-are/algo-lib/blob/master/strings/knuth_morris_pratt.cpp)
Given a string `S`, computes for each prefix the longest proper suffix which is also a prefix of `S`. Supports:
 * Characterizing occurrences (and partial occurrences) of `S` in a specified pattern `T` in `O(|T|)`
 
Precomputation is `O(|S|)`.

 __Usage Examples:__
 * [Pattern Find](https://github.com/saketh-are/algo-lib/blob/master/test/strings/knuth_morris_pratt/pattern_find/pattern_find.cpp)

### [Z Algorithm](https://github.com/saketh-are/algo-lib/blob/master/strings/z_algorithm.cpp)
Given a string `S`, computes the length of the longest common prefix between `S` and each of its suffixes.

Runs in `O(|S|)`.

 __Usage Examples:__
 * [Z Algorithm](https://github.com/saketh-are/algo-lib/blob/master/test/strings/z_algorithm/z_algorithm/z_algorithm.cpp)

### [Palindromes](https://github.com/saketh-are/algo-lib/blob/master/strings/palindromes.cpp)
Given a string `S`:
 * Computes the length of the longest palindrome centered at each character
 * Computes the length of the longest palindrome centered between each pair of adjacent characters
 * Supports queries for the length of the longest palindrome within a specified substring of `S` in `O(log(|S|))`

Precomputation is `O(|S|)`. Additional precomputation for substring queries is `O(|S| log(|S|))`.

 __Usage Examples:__
 * [Enumerate Palindromes](https://github.com/saketh-are/algo-lib/blob/master/test/strings/palindromes/enumerate_palindromes/enumerate_palindromes.cpp)

### [Polynomial Hash](https://github.com/saketh-are/algo-lib/blob/master/strings/polynomial_hash.cpp)
A rolling hash for probabilistic string equality checks. Supports:
 * Hashing a character in `O(1)`
 * Concatenating two hashes in `O(1)`
 * Computing hashes for all prefixes of a given string `S` in `O(|S|)`
 * Given prefix hashes for a string `S`, extracting the hash for an arbitrary substring of `S` in `O(1)`

Increasing parameter _EvaluationPoints_ lowers the probability of two distinct strings producing the same hash.

 __Usage Examples:__
 * [Good Substrings](https://github.com/saketh-are/algo-lib/blob/master/test/strings/polynomial_hash/good_substrings/good_substrings.cpp)

### [Suffix Array](https://github.com/saketh-are/algo-lib/blob/master/strings/suffix_array.cpp)
Given a string `S`:
  * Computes the lexicographic ordering of its suffixes
  * Computes the length of longest common prefix between each pair of adjacent suffixes in the lexicographic ordering
  * Supports queries for the longest common prefix between an arbitrary pair of suffixes
  * Supports queries for the range of ranks at which a specified substring occurs
  * Supports queries for the first occurrence of a specified substring

Precomputation is `O(|S| log(|S|))`.

__Usage Examples:__
* [Suffix Array](https://github.com/saketh-are/algo-lib/blob/master/test/strings/suffix_array/suffix_array/suffix_array.cpp)
* [Count Distinct Substrings](https://github.com/saketh-are/algo-lib/blob/master/test/strings/suffix_array/count_distinct_substrings/count_distinct_substrings.cpp)

### [Suffix Automaton](https://github.com/saketh-are/algo-lib/blob/master/strings/suffix_automaton.cpp)
Given a string `S`, computes the minimal finite automaton accepting all suffixes of `S`.
* Supports queries for the first occurrence in `S` of a specified pattern `T` in `O(|T|)`
* Supports queries for the number of occurrences in `S` of a specified pattern `T` in `O(|T|)`

Precomputation is `O(|S|)`.

__Usage Examples:__
* [Count Distinct Substrings](https://github.com/saketh-are/algo-lib/blob/master/test/strings/suffix_automaton/count_distinct_substrings/count_distinct_substrings.cpp)

### [Trie](https://github.com/saketh-are/algo-lib/blob/master/strings/trie.cpp)
Given a collection of strings `S`, supports:
* Computing the total number of occurrences over all strings in `S` within a specified pattern `T` in `O(|T|)`
* Computing the number of occurrences of each string in `S` within a specified pattern `T` in `O(|S| + |T|)`
* Finding the indices of all occurrences of each string in `S` within a specified pattern `T` in `O(|T| + |S| + total number of occurrences)`

Precomputation is linear in the sum of the lengths of the strings in `S`.

__Usage Examples:__
* [Minimum Length Substring With K Occurrences](https://github.com/saketh-are/algo-lib/blob/master/test/strings/trie/minimum_length_substring_with_k_occurrences/minimum_length_substring_with_k_occurrences.cpp)

### [Mutable String](https://github.com/saketh-are/algo-lib/blob/master/strings/mutable_string.cpp)
Maintains a string over a finite alphabet, supporting:
* Modification of a specified character in `O(sqrt(|S|))`
* Queries for the number of appearances of a specified pattern `T` in a specified substring of `S` in `O(|T|sqrt(|S|))`

__Usage Examples:__
* [Count Matches in Mutable Text](https://github.com/saketh-are/algo-lib/blob/master/test/strings/mutable_string/count_matches_in_mutable_text/count_matches_in_mutable_text.cpp)

### [Mutable String (Bitset)](https://github.com/saketh-are/algo-lib/blob/master/strings/mutable_string_bitset.cpp)
Maintains a string over a finite alphabet, supporting:
* Modification of a specified character in `O(1)`
* Queries for the number of appearances of a specified pattern `T` in a specified substring of `S` in `O(|T| * |S| / MACHINE_WORD_SIZE)`

__Usage Examples:__
* [Count Matches in Mutable Text](https://github.com/saketh-are/algo-lib/blob/master/test/strings/mutable_string_bitset/count_matches_in_mutable_text/count_matches_in_mutable_text.cpp)

`input_gen.py` generates a file with random data values matching a given spec.

The spec generally takes the format `var_name # data_type <additional specifiers>`.

Writing these specs is much faster, easier and less error-prone than writing an equivalent generator from scratch.

## First Examples
A random input to the problem [Watermelon](https://codeforces.com/problemset/problem/4/A) can be specified with:
```
W # int [1, 100]
```
A random input to the problem [Theatre Square](https://codeforces.com/problemset/problem/1/A) can be specified with:
```
# n int [1, 1e9]
# m int [1, 1e9]
# a int [1, 1e9]
n m a
```
or more compactly with:
```
n m a # int [1, 1e9]
```
This spec for the problem [Add Plus Minus Sign](https://codeforces.com/contest/1774/problem/A) makes use of multitest and string support:
```
T # cases [1, 2000]
N # int [2, 100]
a # string N [01]
```
This spec for [Two Chess Pieces](https://codeforces.com/contest/1774/problem/E) makes use of tree support, vector support, and enforces `d <= n`:
```
# n int [2, 2e5]
# d int [2, n]
n d
ui vi # tree n
# m1 m2 int [1, n]
ai # ints m1 [1, n] distinct
bi # ints m2 [1, n] distinct
```

## Workflows
Write an input spec in a plain text file, e.g. `spec`, then:
- Run `python2 input_gen.py spec` to produce a random input on standard out
- Run `python2 input_gen.py spec > inp` to redirect the output to a file `inp`

Generating random inputs and running your program on them can be useful on its own as a sanity check. You can make sure your program runs as quickly as expected and that no runtime errors occur.

If you have an oracle capable of generating expected _outputs_ (perhaps a brute force implementation), you can test your program against it:

```
$ touch out1 out2
$ while diff -q out1 out2; do python2 input_gen.py spec > in; ./prog < in > out1; ./oracle < in > out2; done
```

If an input on which your program and the oracle differ is generated, the loop will terminate. The input of interest will remain available in file `in`.

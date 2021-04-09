# bitarray

An implementation of a bit array, a kind of cross between `std::array` and `std::vector<bool>`

This is refactored code from an older project (in which it started out as being merely 64 bit integers), code to
solve polyomino tilings problems, the rest of which will also be refactored at some point.

## Background

### Other Implementations

The author also considered (and so should you):

- [`std::vector<bool>`](https://en.cppreference.com/w/cpp/container/vector_bool)
- [`Qt::QBitArray`](https://doc.qt.io/qt-5/qbitarray.html)
- [`boost::dynamic_bitset`](https://www.boost.org/doc/libs/1_75_0/libs/dynamic_bitset/dynamic_bitset.html)
- SGI's [`bitset`](https://boost.org/sgi/stl/bitset.html)
- and something else; will remember at some point

### Implementation choice

Originally the author chose 64 bit integers and then wondered why it was necessary to write

```
    int k = 0;
    while((c & 1) == 0) {
	c >>= 1;
	++k;
    }
    return k;
```

to find the LSB of `c` when an x86 CPU has `bsf` (bit scan forward) and could do the thing more efficiently.  Similarly
compilers don't seem to consistently optimise testing bit `k` of `c` where one would write `(c & (1UL << k)) !=0 ` and
again compilers don't generally seem to recognise the pattern and use the CPU native `bt` (bit test).

So the choice behind this code was the desire for something like SGI's bitset but brought up to C++20, and something that
would be simple enough that assembly optimisations could be explored.

### Implementation details

This implementation is pretty firmly based on `u_int32_t` arrays.

The test code is generated, but of course it is not necessary for the user of the library to regenerate the test code as it
is itself checked into the repository.  To regenerate it, though, it is useful to have a general purpose language that handles
arbitratily bit integers natively.  The author knows of two such languages - python and Common Lisp.  Easy choice.

The test code in fact relies on using [SBCL](https://www.cliki.net/SBCL) since SBCL helpfully resets `*random-state*`
to a known fixed state at startup.  In turn, some regression tests of the test generating code are defined but
these are not comprehensive.

The test itself uses catch2.

## Software status

### Portability and standards compliance

The code is an early version so it built/tested only with g++ 10.2

### TODO

- The SIZE should probably be given in bits rather than dwords.
- More regression/unit tests are needed.

### Changelog



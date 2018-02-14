# Rolling Median Kernel Module (RMKM)

This software is an attempt to solve a programming assignment for a job interview.

## The Task

Goal: implement a loadable Linux kernel module for a pseudo-device that can calculate a median of received integers.

On input, the device accepts an arbitrary number of unsorted integers separated by whitespace characters. The device assumes that input data is always valid.

The output is a float number, equal to a median of integers received by the device so far, followed by `EOF`. If a median is impossible to compute, `NaN` is returned.

## The Solution

### Variant 1: Static Array of Counters

Instead of storing every incoming number separately in memory, we maintain a static array, where we count occurences of numbers on the input under corresponding indexes in the array (possibly with offset, to handle negative numbers). So inserting a new number is accomplished by simply incrementing the counter under corresponding index. This makes insertion an operation of _O(1)_ complexity.

Finding a median requires traversing our (always sorted) array and summing the counters, until we reach the half of the count of numbers received so far. As the array has a constant size, finding the median is also _O(1)_ operation.

The catch: this algorithm is only practical for applications where the range of possible input data is known upfront, and decently limited. While it should be fine for many systems to maintain an array of 16KiB size to cover the entire 16-bit integer range, maintaining a 4GiB array (to support 32-bit integer range) would be a venture which only few contemporary systems could afford to undertake.

### Variant 2: Two Equal-Sized Heaps

We maintain two binary heaps: left (`heap_l`) and right (`heap_r`). The left heap holds the lower half of numbers received so far, and the right heap stores the higher half. `heap_l` is a max-heap, i.e. the element with the highest value is the root node. `heap_r` is a min-heap, so the lowest-value element is the root.

With such setup, the two heaps keep our set of numbers received so far always sorted and divided in half, exactly at the median position (we allow numbers of elements in each heap to differ by at most 1). So we can always tell what the current median is, by peeking at the root nodes of two heaps, which is done in _O(1)_ time.

The main contributor to the time complexity is the process of inserting new elements to heaps while keeping them sorted and balanced. An insertion of a new node to the heap (pushing) has _O(_ log _n)_ complexity. On average, half of all insertions will violate the invariant of keeping the heaps equal-sized, which will require popping the root from one heap and pushing it to the other. Popping is also an operation of _O(_ log _n)_ complexity.

To summarize:

- _O(_ log _n)_ to deal with every single number on the input, which expands to...
- _O(n_ log _n)_ to read _n_ numbers from the input
- _O(1)_ to get the median at any time.


## Building and Running

### Prerequisites
- Make sure you have all the standards tools and utilities for kernel development: gcc, linux-headers, etc.
- If you want to run tests automatically, install [Bats test framework](https://github.com/bats-core/bats-core) (optional).

### Building

To build the variant using heap algorithm (default):
```
make
```

To build the variant using static array algorithm:
```
make ALGO=static
```

### Running

To run automatic tests, run:
```
make test
```

To interact with the module manually (insmod and rmmod require root access):
```
> make insmod
> cat /dev/median
NaN

> echo 3 7 12 5 5 4 > /dev/median
> cat /dev/median
5.0

> make reload
reloading rmkm module

> echo 1 3 5 > /dev/median
> cat /dev/median
3.0

> make rmmod
```

There are also convenience shell functions (`feed` and `ans`) for manual testing:
```
> source ./src/utils.sh
> make insmod
> feed 1 2 3
> ans
2.0
> feed 4 5
> ans
3.0
```

## Known Issues

- The module cannot yet handle input data spanning over multiple buffers. The input buffer is set to be very small (32B) to make the module fail fast, rather than give an impression that it can accept bigger chunks of data. When data is supplied in small chunks however, everything works just fine.

- The module doesn't yet reallocate memory progressively. It allocates just a single page (4KiB on a typical desktop system) for each heap. When one of the heaps will try to grow over 4KiB, the module will fail.

- The module has not been stress-tested yet, so it may fail in multiple interesting ways.

- In the static array variant, the module does not signal counter overflow. So after supplying any number more than 2^32 times, the counter will overflow to 0, but the module will happily keep returning invalid medians when asked.

## Roadmap

- [x] Set up a repo
- [x] Set up a coding style
- [x] Set up a Makefile
- [x] Create a "hello world" kernel module
- [x] Create a character device (manually)
- [x] Create/Destroy /dev/median device (automatically)
- [x] Make /dev/median consume and produce data
- [x] Make the device actually calculate a median
- [x] Handle negative integers
- [x] Handle float results
- [x] Return NaN when no input yet
- [x] Set up a testing framework
- [x] Add basic tests (simple enough for a human to generate and verify)
- [x] Research algorithms
- [x] Implement an optimal algorithm
- [x] Describe the solution in readme
- [ ] Allocate more memory when needed
- [ ] Handle large chunks of input data
- [ ] Add stress tests
- [ ] Polish the code for better maintainability


## References

- [The Linux Kernel Module Programming Guide](http://tldp.org/LDP/lkmpg/2.6/html/)
- [The Linux driver implementer’s API guide](https://www.kernel.org/doc/html/v4.15/driver-api/index.html)
- https://opensourceforu.com/tag/linux-device-drivers-series/
- http://www.ardendertat.com/2011/11/03/programming-interview-questions-13-median-of-integer-stream/

## License

Copyright © 2018 Cezary Sałbut. All rights reserved.

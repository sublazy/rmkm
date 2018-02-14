# Rolling Median Kernel Module (RMKM)

This software is an attempt to solve a programming assignment for a job interview.

**Note to reviewers: this assignment is not finished yet; please wait with evaluation until it's done.**

## The Task

Goal: implement a loadable Linux kernel module for a pseudo-device that can calculate a median of received integers.

On input, the device accepts an arbitrary number of unsorted integers separated by whitespace characters. The device assumes that input data is always valid.

The output is a float number, equal to a median of integers received by the device so far, followed by `EOF`. If a median is impossible to compute, `NaN` is returned.

## The Solution

TODO: Describe

## Building and Running

- install linux-headers
- install [Bats test framework](https://github.com/sstephenson/bats) (optional).

TODO: Describe

## Known Issues

- The module cannot yet handle input data spanning over multiple buffers. The input buffer is set to be very small (32B) to make the module fail fast, rather than give an impression that it can accept bigger chunks of data. When data is supplied in small chunks, everything works just fine.

- The module doesn't yet reallocate memory progressively. It allocates just a single page (4KiB on a typical desktop system) for each heap. When one of the heaps will try to grow over 4KiB, the module will fail.

- The module has not been stress-tested yet, so it may fail in multiple interesting ways.

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
- [ ] Describe the solution in readme
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

# Rolling Median Kernel Module (RMKM)

This software is an attempt to solve a programming assignment for a job interview.

**Note to reviewers: this assignment is not finished yet; please wait with evaluation until it's done.**

## The Task

Goal: implement a loadable Linux kernel module for a pseudo-device that can calculate a median of received integers.

On input, the device accepts an arbitrary number of unsorted integers separated by whitespace characters. The device assumes that input data is always valid.

The output is a float number, equal to a median of integers received by the device so far, followed by `EOF` character. If a median is impossible to compute, `NaN` is returned.

## TODOs

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
- [ ] Handle large chunks of input data
- [ ] Set up a testing framework
- [ ] Add basic tests (simple enough for a human to generate and verify)
- [ ] Research algorithms
- [ ] Implement an optimal algorithm
- [ ] Make it work
- [ ] Make it right
- [ ] Add more serious tests
- [ ] Make it fast (or otherwise optimal)
- [ ] Add stress tests
- [ ] Polish the code for better maintainability

## References

- [The Linux Kernel Module Programming Guide](http://tldp.org/LDP/lkmpg/2.6/html/)
- [The Linux driver implementer’s API guide](https://www.kernel.org/doc/html/v4.15/driver-api/index.html)
- https://opensourceforu.com/tag/linux-device-drivers-series/

## License

Copyright © 2018 Cezary Sałbut. All rights reserved.

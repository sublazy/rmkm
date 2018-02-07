# Rolling Median Kernel Module (RMKM)

This software is an attempt to solve a programming assignment for a job interview.

**Note to reviewers: this assignment is not finished yet; please wait with evaulation until it's done.**

## The Task

Goal: implement a loadable Linux kernel module for a pseudo-device that can calculate a median of received integers.

On input, the device accepts an arbitrary number of unsorted integers separated by whitespace characters. The device assumes that input data is always valid.

The output is a float number, equal to a median of integers received by the device so far, followed by `EOF` character. If a median is impossible to compute, `NaN` is returned.

## TODOs

- [x] Set up a repo
- [ ] Set up Linux kernel coding style
- [ ] Set up a Makefile 
- [ ] Create a "hello world" kernel module
- [ ] Set up a testing framework
- [ ] Add basic tests (simple enough for a human to generate and verify)
- [ ] Research algorithms
- [ ] Implement an optimal algorithm
- [ ] Make it work
- [ ] Make it right
- [ ] Add more serious tests
- [ ] Make it fast (or otherwise optimal)
- [ ] Add stress tests

## License

Copyright © 2018 Cezary Sałbut. All rights reserved. 

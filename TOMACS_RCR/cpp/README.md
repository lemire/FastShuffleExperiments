
Fast random shuffling experiments
===================================

This code assumes that you have either a Linux or macOS system with a C++ compiler such as GNU GCC or LLVM's clang. We provide a make file so usage should be as simple as typing:

```
make
make test
```

We also include some statistical tests (``stattests``) which serves a form of unit testing, to make sure that there is no software bug in the implementations.

It is meant to be used on x64 processors.

There is a blog post describing the core idea: http://lemire.me/blog/2016/06/30/fast-random-shuffling/

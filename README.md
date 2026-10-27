# Project 2 README

Student 1: Huanchen Ji (927576963)

Student 2: Omkar Toro (921123231)

## Project Status
Implemented union-find, a hash map, and friend-group management.
All 43 tests pass. Line coverage is 100% (131/131 lines).
The Makefile builds the project, runs tests, generates HTML coverage,
checks memory with Valgrind, and runs all four provided datasets.
Valgrind reported zero errors in the checks performed.

## Known Issues
LCOV reports mismatched-end-line warnings for GoogleTest-generated code,
but successfully generates the coverage report.
The medium dataset reports 3,926 connections, while the instructor
analysis table lists 3,927.

## Code References
* [std::vector::push_back](https://en.cppreference.com/w/cpp/container/vector/push_back)

* [std::list::splice — moving nodes between hash buckets](https://en.cppreference.com/w/cpp/container/list/splice)

## Generative AI Use
I used ChatGPT for Prompt:Linker error, and I used ChatGPT for the interactions documented below.

### Prompt: Linker error

只有一处

Context: I searched for Connected in MyUnionFind.h and found only
its declaration after encountering an undefined-reference error.

### Response

ChatGPT provided this missing definition:

```cpp
inline bool CMyUnionFind::Connected(TElementID a, TElementID b) {
    TGroupID RootA = Find(a);
    TGroupID RootB = Find(b);

    if (RootA == InvalidGroupID || RootB == InvalidGroupID) {
        return false;
    }

    return RootA == RootB;
}
```

### Changes and rationale

I added the definition before #endif in MyUnionFind.h.
The subsequent run passed all 18 provided union-find tests.


### Prompt 2
How can I move nodes between two std::list containers without copying them?

### Response 2
std::list<int> Source{10, 20};
std::list<int> Destination;

auto It = Source.begin();
Destination.splice(Destination.end(), Source, It);

### Changes 2
No code changes were made. I used this explanation to understand
how splice transfers existing nodes between buckets in Rehash()
without copying their keys and values.
Main takeaways:
1. Part 1:
- Although this is a graph problem, since the depth is limited to 1, and the cycles are limited to 3, an imperative approach with no while loops or recursion works great;
- Tuples are ordered, so sorting them avoids duplication in sets, if order does not matter for the problem;
- C: implementation of a 2-level table, with the first level holding pointers to the second level. This saves a bunch of memory for sparse data, just like hierarchical page tables in operating systems. It also allows constant time checking if a PC exists in the network by checking if the pointer is null, instead of scanning for all of its possible connections. This is done with no dynamic memory allocation, only an arena;
- C: hash set implementation;
- C: converting a length 2 string into a short ranging from 0 to 625;
2. Part 2:
- The problem was clearly asking for the maximal clique in the graph. There's value in being able to code up a maximal clique solution, but there's also value in just *recognizing* that it is a maximal clique problem. I patted myself on the back for the pattern matching ability and used a library;
- Bron-Kerbosch algorithm implementation.

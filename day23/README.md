Main takeaways:
1. Part 1:
- Although this is a graph problem, since the depth is limited to 1, and the cycles are limited to 3, an imperative approach with no loops or recursion works great;
- Tuples are ordered, so sorting them avoids duplication in sets, if order does not matter for the problem.
2. Part 2:
- The problem was clearly asking for the maximal clique in the graph. There's value in being able to code up a maximal clique solution, but there's also value in just *recognizing* that it is a maximal clique problem. I patted myself on the back for the pattern matching ability and used a library;
- Bron-Kerbosch algorithm implementation.

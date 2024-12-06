Part two of this challenge basically requires part one, so I didn't split the files. Instead, I decided it would be fun to solve it using dependency graphs both in the form of an adjacency list and the form of an adjacency matrix (TODO). Besides of the list / matrix approach, I solved it using an inverted graph, and also by doing a topological sort first, then checking if the ordering of the pages respects the topological sort (which is the classier approach, to be fair).

Main takeaways:
- List:
  - Really interesting implementation of adjacency lists using linked lists allocated in an arena inside the same struct that holds the heads of the lists. Knowing exactly how many nodes there will be allow nodes to be allocated statically;
  - Sentinel nil node instead of using null pointers;
  - Zero is initialization;
  - "Fixed point" style algorithm to find if prereqs have been met (the loop eventually converges to increments and decrements cancelling out);
  - Building adjacency lists out of edges.
- Matrix:
  - A lot easier build;
  - TODO

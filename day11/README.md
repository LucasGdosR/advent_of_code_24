Main takeaways:
Both parts are the exact same problem, but part 1 is solvable with brute force, and part 2 requires memoization.

Part 1:
I modeled the problem after the question and made a doubly-linked list with two dummy / sentinel nodes, the head and the tail.
- Faithful simulation of the problem;
- Ability to introspect and check the whole line of stones at any given moment;
- Logarithm and exponential math for manipulating decimal digits in an integer.

Part 2:
I only cared about the count of stones, which is what was being asked. This allowed me to use memoization to cache the count that a stone eventually should get to.
- Memoization implementation;
- Same math.

TODO:
C version. Basically implement a hash table for tuples, and a doubly linked list.

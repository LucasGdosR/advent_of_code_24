Main takeaways:
Both parts are the exact same problem, but part 1 is solvable with brute force, and part 2 requires memoization.

Part 1:
I modeled the problem after the question and made a doubly-linked list with two dummy / sentinel nodes, the head and the tail.
- In the C implementation, I used uint32 as pointers into an arena. As long as we manage our own memory, we don't need to use 64-bit pointers, only as large as our memory address requires. Since I was using an arena of my own that could fit up to 200,000 elements, 32-bits were plenty. Know your data;
- Arenas for static memory allocation of linked list nodes;
- Faithful simulation of the problem;
- Ability to introspect and check the whole line of stones at any given moment;
- Logarithm and exponential math for manipulating decimal digits in an integer.

Part 2:
I only cared about the stone count, which is what was being asked. This allowed me to use memoization to cache the count that a stone eventually should get to.
- Memoization implementation;
- Same math.

TODO:
C version of part 2. Basically implement a hash table for two numbers as keys.

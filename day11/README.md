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
- Memoization implementation using a chained hash table;
- Know your data: resizing a hash table once it runs out of capacity is expensive. Every input needs to be rehashed. If you know the needed capacity, you don't need to resize and rehash, and you can statically allocate, ensuring better cache locality for chained hash tables;
- Use of a static arena for the chained hash table, avoiding rehashing the whole table when it becomes too full, and ensuring better cache locality for the linked lists;
- Cool macros;
- `blink_n_times` has a single point of exit, which is handy for profiling and stuff. I still think it's less readable than early returning, but it can be useful;
- Same math.

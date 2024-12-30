Main takeaways:

Part 1:
- Pretty simple iterative solution. Could be done recursively instead, but the need to mutate only the first and last elements works better with iteration. Recursion would probably mutate them all for naught.

Part 2:
- Recursive check for vertical movement validity ("depth first search");
- ~Iterative level movement with a queue ordering levels ("breadth first search")~;
- ~Deque implementation in C with a circular buffer that uses a bit mask for wrapping around~;
- Both of the previous points are available in the commit history. However, I later realized that this is a matter of post-order vs pre-order. The movement can be processed depth first, as long as the leaves are processed before their parents. This led to a recursive post-order solution.

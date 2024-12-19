Main takeaways:

Dynamic programming:
- Split into smaller subproblems. If the beginning can be formed, see if the rest can be formed recursively. Count how many times they can be formed for part 2;
- Memoize the answer in a cache.

C version:
- Part 1 was implemented for free using part 2's count as a boolean with `!!`;
- Implementation of a linked hash map built on top of an arena, with no pointers inside the nodes, only integers that are offsets into the arena;
- This linked hash map appends elements to the end of the list. This is useful when the cases that occur first are more likely, ensuring that looking them up later results in less chain traversals than if the new elements were added to the list's head. This is unlikely to be beneficial in this particular case, but it can be useful to have in your toolbelt;
- Implementation of string operations such as `startswith` and a constant time `len`. C's `strlen` takes linear time in the length of the string.

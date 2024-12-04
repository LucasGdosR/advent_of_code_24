The main takeaways from this day are:
1. Part 1:
- Know your data. With a known (or at least bounded) input size, there's no need for dynamic memory allocation;
- Quicksort: 3-way implementation that's optimized for dealing with repeated values.
2. Part 2:
- Know your data. If your input is uniformly distributed, there's no need to hash it to store in a hashtable. You can just use the input mod the size of the hashtable as a key;
- Hashtable with linked lists at each slot.
- With a bounded allocation for the linked lists on the hashtable, they can all be allocated from an arena that's contiguous in memory;
- Instead of dealing with null pointers and risking dereferencing them, use a sentinel NIL_NODE that self references and can be dereferenced safely.

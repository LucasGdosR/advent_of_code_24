Main takeaways:
Python isn't too suited to this kind of problem. Array indices are the key here. While that's harsh for a pythonista, that's just another day in the life of a C programmer. I felt like I was writing C in Python, except for the list comprehensions.

Part 1:
Here I represented the file system explicitly. I represented each block as an element of an array.
- Mapping a file to memory and reading it as an array of bytes;
- Using non-negative numbers for storing quantity (file id), and negative numbers as a kind of boolean flag (meaning the block is free);
- Using the knowledge of the final occupied space to quit iterating early in the checksum, and to not bother cleaning up file ids from places in the file system that won't be read for calculating the checksum;
- In Python, the input was read one byte at a time, and treated differently if it was an even or odd index. In C, 2 bytes were read at a time, and each was treated as it should be. This eliminates the need for branching and reduces the number of iterations by half. This actually makes a huge difference. TODO: measure how much difference.

Part 2:
Here I represented the file system implicitly. It is represented by four arrays, file sizes, file addresses, free space sizes, and free space addresses. This implicit representation is the more compact, better one. It could be more neatly represented as a Struct of Arrays (SoA), but I didn't bother.
- Caches to really speed up the algorithm. With only 20 bytes of cache, the algorithm goes from O(n²) to O(n);
- Using integers as pointers. A pointer is just an offset to a base address. Usually that base address is 0 in the virtual address space. In this implementation, the integers are an offset to the base address of the file system. No need to call them `*` for them to be pointers semantically;
- Using math identities instead of loops. In the first part, the code goes over the file system and sums all `file_id * address`. In this part, I multiply the number of blocks in a file by the file id and the average of its index, and this dramatically reduces the number of iterations depending on the case;
- I was inspired by reading 2 bytes at a time to make some silly loop-unrolling-like implementation that reads 8 bytes at a time and extracts 8 1-byte values from it. TO DO: measure the difference between reading 1 / 2 / 4 / 8 bytes at a time.

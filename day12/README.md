Main takeaways:

Part 1:
- Graph search finding components;
- Calculating the perimeter of a graph;
- I implemented a union that allowed me to use a single array to store a generation number (`REGION_ID`), the array's size (`REGION_SIZE`), and a collection of elements (using the size to determine where to store). This array is also an arena, and resetting the element at `REGION_SIZE` is akin to freeing a stack frame and starting anew, with no need to clear all the memory contents or free and allocate new memory;
- Using generations allowed me to use a grid of "booleans" for the entire dataset, while also keeping information about who is a neighbor to whom;
- Macro for succintly dealing with multiple neighbors.

Part 2:
- This was the first hard problem in this year's edition. Implementing the solution is easy, but realizing that in order to count sides, you should count corners, and that to count corners, you should look for indexes that appear an odd number of times, is tricky. The edge cases of corners that look like they touch, but don't, add another tricky detail.

TO DO: part 2 C version

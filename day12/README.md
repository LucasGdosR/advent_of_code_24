Main takeaways:

Part 1:
- Graph search finding components;
- Calculating the perimeter of a graph;
- I implemented a union that allowed me to use a single array to store the array's size at using a 16-bit uint the header (`REGION_SIZE`), and a collection of points with 2 8-bit uint coordinates (using the size to determine where to store). This array is also an arena, and resetting the element at `REGION_SIZE` is akin to freeing a stack frame and starting anew, with no need to clear all the memory contents or free and allocate new memory;
- Since all nodes will eventually be visited, using a matrix the same size as the `garden` for marking visited nodes is a better alternative than a hash set;
- Macro for succintly dealing with multiple neighbors.

Part 2:
- This was the first hard problem in this year's edition. Implementing the solution is easy, but realizing that in order to count sides, you should count corners, and that to count corners, you should look for indexes that appear an odd number of times, is tricky. The edge cases of corners that look like they touch, but don't, add another tricky detail.

TO DO: part 2 C version

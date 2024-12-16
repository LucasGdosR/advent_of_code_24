Main takeaways:

Part 1:
- Graph search finding components;
- Calculating the perimeter of a graph;
- I implemented a union that allowed me to use a single array to store the array's size at using a 16-bit uint the header (`REGION_SIZE`), and a collection of points with 2 8-bit uint coordinates (using the size to determine where to store). This array is also an arena, and resetting the element at `REGION_SIZE` is akin to freeing a stack frame and starting anew, with no need to clear all the memory contents or free and allocate new memory;
- Since all nodes will eventually be visited, using a matrix the same size as the `garden` for marking visited nodes is a better alternative than a hash set;
- Macro for succintly dealing with multiple neighbors.

Part 2:

This was the first hard problem in this year's edition. Implementing the solution is easy, but realizing that in order to count sides, you should count corners, and that to count corners, you should look for indexes that appear an odd number of times, is tricky. The edge cases of corners that look like they touch, but don't, add another tricky detail.
- The union used in part 1 now also holds an `id`, which is used to check if two tiles with the same plant that are diagonally next to each other are part of the same region or not. This is a use of "generations";
- By using generations, the `visited` array can't hold 8-bit integers, because there are more than 256 generations. It must hold at least 16-bit integers. With that, I used the least significant bit as a boolean, and the rest for the actual generation. This LSB is toggled and untoggled with XOR operations with `1`, counting whether they are applied an even or odd number of times;
- Branchless code that sums an integer multiplied by a chain of conditionals;
- The `visited` matrix is mutated outside of the bounds of the regions. With that, it's not possible to simply iterate i and j from the region's points. I implemented maintaining a rectangular bound on the region, and then scanning the rectangular region. Another solution would be to check 4 points for every point in the region. Depending on the shape of the regions, one might be better than the other.

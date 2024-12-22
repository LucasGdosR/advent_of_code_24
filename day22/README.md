Main takeaways:
1. Part 1:
- Pretty much a tutorial on generating pseudorandom numbers. Start with a number, multiply or divide by a power of two (shift it left or right), mix it with the original number (XOR them), and use a modulo operation to get something in your range (usually use a bitmask with a sequence of 0s followed by a sequence of 1s).
2. Part 2:
- Dynamic programming: calculate how much each sequence would profit in a single pass by storing the price when a sequence is first seen in a stream of prices, then get the max profit;
- Implementation of a buffered queue tailored to the 4-sequence tracking problem;
- Split a 0->2000 loop into 0->4 and 4->2000, getting rid of conditional checks inside the loop;
- The 0->4 loop is not really a loop. I unrolled it with a macro;
- Custom hash function that spreads the inputs perfectly with zero collisions.

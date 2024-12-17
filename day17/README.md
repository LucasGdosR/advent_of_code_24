Main takeaways:

Simulating a machine that runs some bytecode is a matter of implementing functions for each operation, and matching the opcode to find which function to call. This can be done rather trivially and efficiently with a table look-up. In C, I used a switch-statement, and in Python, an explicit table with functions.

Part 2 requires understanding the machine / the program. My input basically loops over the contents of register A 3 bits at a time, and mixes the most significant bits (MSB) and least significant bits (LSB). The 3 MSB determine the last output character, since at the last iteration, all that's left are the 3 original MSB, and there's nothing to mix. With fixed 3 MSB, the next 3 bits determine the penultimate output, and so forth. This dependency chain goes on untill the whole output is determined.

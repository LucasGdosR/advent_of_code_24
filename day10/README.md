Main takeaways:
It was actually pretty cool that in C I could just compare a (char + 1) to its neighbor, but in Python I needed to convert strings into ints. C isn't always longer to type than Python.

Part 1:
- Depth-first search in a grid;
- Sentinels to avoid bounds checking;
- In C, using a matrix of generations as booleans that can be reused (`hilltop_reached`). You can check if the value in the array is equal to an identifier that's unique to the iteration, with the semantics of true/false. There's no need to reset the matrix to `false` in each iteration, just change the generation (the number that's compared to the matrix's elements);
- I did some bit operations to fit two 8-bits numbers inside a single 16-bits and pass it as a parameter. This has no particular utility, and is essentially a struct;
- In Python, making a list/tuple to easily iterate through directions, and using a set to accumulate unique `HILLTOP`s reached. The set requires a fresh state before each iteration, unlike the matrix of generations in C.

Part 2:
- No matrix or set, just a recursive function that walks the path and sums the paths for all of its children. Could instead be implemented with a global variable or dereferenced pointer that's explicitly incremented inside the function instead of accumulating.

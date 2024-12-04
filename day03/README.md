Main takeaways:
- Choosing the right interface for the input: in this challenge, it is better to deal with a single memory map than separating the input by lines;
- Explicit state machine implementation with enums for state and switch statement. This is actually efficient, as the compiler can use a jump table to determine the case, with no need for multiple conditional checks;
- Parsing strings to integers by hand efficiently;
- Using `#define` for repeated code;
- For explicit regex solution, check the Python implementation.

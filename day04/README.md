Main takeaways:
1. Part 1:
-  Sentinels can be used to prevent edge cases, such as bound checking;
-  Sentinels may lead to off-by-one errors if not careful;
-  Global variables are fine in some cases so pointers don't need to be passed around;
-  Global arrays are initialized as all zeroes by default in C, so zero is the sentinel;
-  Using a "directions" / "neighbors" matrix is handy for search problems in grids.
2. Part 2:
-  Sentinels aren't needed if you're not going to the boundaries;
-  Disregarding linting may lead to pretty and readable code.

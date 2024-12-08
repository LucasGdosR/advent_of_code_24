Main takeaways:
- Hashtable with zero collisions and dynamic vector in its slots: pretty easy to hash a single letter (it's just the ASCII value), pretty easy to avoid collisions (just make it as big as letters gets), and dynamic vectors are pretty cool. Feels good to just iterate over a known size in C;
- Ignoring irrelevant edge cases: if you're iterating over a size which might be zero, there's no need to put checks that the iterator isn't empty. Size zero makes zero iterations, which is the same;
- If some assumption would help you, make it valid (made one point the one with smaller x coordinate, and another with the larger x coordinate);
- Some basic geometry.

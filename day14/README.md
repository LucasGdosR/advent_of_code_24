Main takeaways:

Part 1:
- The problem asks for simulating 100 timesteps. However, it only asks for a snapshot. In this case, it's best to simulate advancing all timesteps at once, instead of iterating each of them.

Part 2:
- In the C version, there's some low-level manipulation of strings. `memset` is used with a 4-byte hexadecimal to fill a matrix with the `'.'` character, and `\n\0` is appended to the end of each row to turn it into a null terminated string including a newline, removing the nested loop;
- This is about prime numbers. A simulation step involves `(position + velocity * duration) % dimension`. Therefore, every `dimension` iterations, the position repeats itself. Both dimensions are prime numbers. You can find the offset by iterating from 0 to the larger dimensions, and then iterating on an offset using the step of the dimension that makes it repeat, starting from the offset and going to `height * width`. I made a terminal viz for this, but it can be redirected to a text file at the terminal via `python3 14-2.py > ans.txt`
![image](https://github.com/user-attachments/assets/1ea3e674-810b-4a42-970f-47d827634d6e)

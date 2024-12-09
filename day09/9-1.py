input = '2333133121414131402'

fs_capacity = sum((int(n) for n in input))

fs_contents = [-1 for _ in range(fs_capacity)]

fs_idx = 0
f_id = 0
fs_size = 0
for i, n in enumerate(input):
    if (i % 2) == 0:
        fs_size += int(n)
        for _ in range(int(n)):
            fs_contents[fs_idx] = f_id
            fs_idx += 1
        f_id += 1
    else:
        fs_idx += int(n)

rev_gen = (fs_contents[i] for i in range(fs_capacity - 1, -1, -1))

for i in range(fs_size):
    if fs_contents[i] == -1:
        e = next(rev_gen)
        while e == -1:
            e = next(rev_gen)
        fs_contents[i] = e

checksum = sum((i * n for i, n in enumerate(fs_contents[:fs_size])))

print(checksum)

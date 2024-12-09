input = '2333133121414131402'

fs_contents = []
file_sizes_and_ptrs = []
f_id = 0
for i, n in enumerate(input):
    if (i % 2) == 0:
        file_sizes_and_ptrs.append((int(n), len(fs_contents)))
        for _ in range(int(n)):
            fs_contents.append(f_id)
        f_id += 1
    else:
        for j in range(-int(n), 0):
            fs_contents.append(j)

search_index_cache = [0] * 10

for f_id, (fsize, fptr) in enumerate(file_sizes_and_ptrs[::-1], -len(file_sizes_and_ptrs) + 1):
    f_id = -f_id
    
    cache = search_index_cache[fsize]
    for i in range(cache, len(fs_contents)):
        if i >= fptr:
            break
        if fs_contents[i] <= -fsize:
            for j in range(fsize):
                fs_contents[i + j] = f_id
                fs_contents[fptr + j] = 0
            for j in range(fsize, 10):
                search_index_cache[j] = i + fsize
            break

checksum = sum((i * n for i, n in enumerate(fs_contents) if n > 0))

print(checksum)

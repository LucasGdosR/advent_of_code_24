keys_n_locks = '''#####
.####
.####
.####
.#.#.
.#...
.....

#####
##.##
.#.##
...##
...#.
...#.
.....

.....
#....
#....
#...#
#.#.#
#.###
#####

.....
.....
#.#..
###..
###.#
###.#
#####

.....
.....
.....
#....
#.#..
#.#.#
#####'''.splitlines()

from collections import defaultdict

def compress_string_to_tuple(key_or_lock):
    return tuple(sum(key_or_lock[i][j] == '#' for i in range(5)) for j in range(5))

keys_by_sum, locks_by_sum = defaultdict(list), defaultdict(list)
for i in range(0, len(keys_n_locks), 8):
    key_or_lock = compress_string_to_tuple(keys_n_locks[i + 1: i + 6])
    if keys_n_locks[i][0] == '#':
        keys_by_sum[sum(key_or_lock)].append(key_or_lock)
    else:
        locks_by_sum[sum(key_or_lock)].append(key_or_lock)

def check_overlap(key, lock):
    return all(key[i] + lock[i] <= 5 for i in range(5))

possible_matches = 0
for i in range(26):
    for key in keys_by_sum[i]:
        for j in range(26 - i):
            for lock in locks_by_sum[j]:
                possible_matches += check_overlap(key, lock)

print(possible_matches)

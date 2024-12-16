maze = '''###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############'''.splitlines()

import heapq

clockwise = {
    (0, 1): (1, 0),
    (1, 0): (0, -1),
    (0, -1): (-1, 0),
    (-1, 0): (0, 1)
}

counterclock = {
    (0, 1): (-1, 0),
    (-1, 0): (0, -1),
    (0, -1): (1, 0),
    (1, 0): (0, 1)
}

heap = None
for i, row in enumerate(maze):
    if heap is None:
        for j, c in enumerate(row):
            if c == 'S':
                heap = [(0, i, j, (0, 1))]
                break

visited = set()
while heap:
    score, i, j, f = heapq.heappop(heap)
    
    fi, fj = i + f[0], j + f[1]
    tile = maze[fi][fj]
    if tile == 'E':
        print(score + 1)
        break
    elif tile == '.':
        forward = (score + 1, fi, fj, f)
        if forward[1:] not in visited:
            visited.add(forward[1:])
            heapq.heappush(heap, forward)

    rotate_clock = (score + 1000, i, j, clockwise[f])
    if rotate_clock[1:] not in visited:
        visited.add(rotate_clock[1:])
        heapq.heappush(heap, rotate_clock)

    rotate_counter = (score + 1000, i, j, counterclock[f])
    if rotate_counter[1:] not in visited:
        visited.add(rotate_counter[1:])
        heapq.heappush(heap, rotate_counter)

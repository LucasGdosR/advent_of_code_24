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

'''
    Mappings for rotations
'''
clockwise = {
    (0, 1): (1, 0),
    (1, 0): (0, -1),
    (0, -1): (-1, 0),
    (-1, 0): (0, 1)
}

counterclock = {k: v for v, k in clockwise.items()}

'''
    Init data structures
'''
for i, row in enumerate(maze):
        for j, c in enumerate(row):
            if c == 'S':
                previous_neighbors_in_shortest_path = {(i, j, (0, 1)): [[], 0]}
                heap = [(0, i, j, (0, 1))]
            if c == 'E':
                end = (i, j)

visited = set()
max_score = float('inf')
score = 0

'''
    Main loop
'''
while score <= max_score:
    score, i, j, f = heapq.heappop(heap)
    
    '''
        Forward step
    '''
    fi, fj = i + f[0], j + f[1]

    # Tracking paths (part 2)
    if (fi, fj, f) not in previous_neighbors_in_shortest_path:
        previous_neighbors_in_shortest_path[(fi, fj, f)] = [[], score + 1]
    if score + 1 == previous_neighbors_in_shortest_path[(fi, fj, f)][1]:
        previous_neighbors_in_shortest_path[(fi, fj, f)][0].append((i, j, f))

    # Dealing with binary heap
    tile = maze[fi][fj]
    if tile == 'E':
        print(score + 1)
        max_score = score + 1
    elif tile == '.':
        forward = (score + 1, fi, fj, f)
        if forward[1:] not in visited:
            visited.add(forward[1:])
            heapq.heappush(heap, forward)
    
    '''
        Rotating neighbors
    '''
    # Tracking paths (part 2)
    if (i, j, clockwise[f]) not in previous_neighbors_in_shortest_path:
        previous_neighbors_in_shortest_path[(i, j, clockwise[f])] = [[], score + 1000]
    if score + 1000 == previous_neighbors_in_shortest_path[(i, j, clockwise[f])][1]:
        previous_neighbors_in_shortest_path[(i, j, clockwise[f])][0].append((i, j, f))

    if (i, j, counterclock[f]) not in previous_neighbors_in_shortest_path:
        previous_neighbors_in_shortest_path[(i, j, counterclock[f])] = [[], score + 1000]
    if score + 1000 == previous_neighbors_in_shortest_path[(i, j, counterclock[f])][1]:
        previous_neighbors_in_shortest_path[(i, j, counterclock[f])][0].append((i, j, f))

    # Dealing with binary heap
    rotate_clock = (score + 1000, i, j, clockwise[f])
    if rotate_clock[1:] not in visited:
        visited.add(rotate_clock[1:])
        heapq.heappush(heap, rotate_clock)

    rotate_counter = (score + 1000, i, j, counterclock[f])
    if rotate_counter[1:] not in visited:
        visited.add(rotate_counter[1:])
        heapq.heappush(heap, rotate_counter)

'''
    Backtrack path
'''
def backtrack_tiles(i, j, f, tiles):
    tiles.add((i, j))
    prev, _ = previous_neighbors_in_shortest_path[(i, j, f)]
    for i, j, f in prev:
        backtrack_tiles(i, j, f, tiles)
    
shortest_path_tiles = set()
for d in clockwise.keys():
    if (end[0], end[1], d) in previous_neighbors_in_shortest_path:
        backtrack_tiles(end[0], end[1], d, shortest_path_tiles)
print(len(shortest_path_tiles))

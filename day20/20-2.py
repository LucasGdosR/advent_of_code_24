racetrack = '''your input here'''.splitlines()

DIRECTIONS = ((-1, 0), (1, 0), (0, 1), (0, -1))

path = {} # key: (i, j); val: steps from start to (i, j)

# Problem is too deep for recursive implementation
def iterative_dfs(i, j, steps):
    stack = [(i, j, steps)]
    while stack:
        i, j, steps = stack.pop()
        for di, dj in DIRECTIONS:
            if racetrack[i+di][j+dj] != '#' and (i+di, j+dj) not in path:
                path[(i+di, j+dj)] = steps + 1
                stack.append((i+di, j+dj, steps+1))

for i, row in enumerate(racetrack):
    for j, c in enumerate(row):
        if c == 'S':
            path[(i, j)] = 0
            iterative_dfs(i, j, 0)

# Cheats that lead to 100+ benefits.
# Elements: ((si, sj), (ei, ej))
great_cheats = set()

from collections import deque

def depth_limited_bfs(si, sj, steps):
    q = deque()
    q.append(((si, sj), 0))
    visited = set(((si, sj)))
    while q:
        (ei, ej), cheating_steps = q.popleft()
        for di, dj in DIRECTIONS:
            dst = (ei+di, ej+dj)
            if (cheating_steps < 20 and
                0 <= ei+di < len(racetrack) and 
                0 <= ej+dj < len(racetrack[0]) and
                dst not in visited
            ):
                visited.add(dst)
                q.append((dst, cheating_steps + 1))
                if dst in path and path[dst] >= steps + 100 + cheating_steps:
                    great_cheats.add(((si, sj), dst))
    
for (i, j), steps in path.items():
    depth_limited_bfs(i, j, steps)

print(len(great_cheats))

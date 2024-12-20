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
    q.append(((si, sj)))
    visited = set((si, sj))
    for cheating_steps in range(20):
        for _ in range(len(q)):
            (ei, ej) = q.popleft()
            for di, dj in DIRECTIONS:
                dst = (ei + di, ej + dj)
                if dst not in visited:
                    visited.add(dst)
                    q.append(dst)
                    if path.get(dst, 0) >= steps + 100 + cheating_steps:
                        great_cheats.add(((si, sj), dst))

for (i, j), steps in path.items():
    depth_limited_bfs(i, j, steps)

print(len(great_cheats), end - start)

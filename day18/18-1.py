falling_bytes = '''your input here'''.splitlines()

from collections import deque

NEIGHBORS = ((-1, 0), (0, -1), (0, 1), (1, 0))

grid = [[True for _ in range(71)] for _ in range(71)]

for byte in falling_bytes[:1024]:
    i, j = byte.split(',')
    grid[int(i)][int(j)] = False

q = deque()

# i, j, steps
q.append((0, 0, 0))

while q:
    i, j, steps = q.popleft()
    if i == 70 and j == 70:
        print(steps)
        break

    for di, dj in NEIGHBORS:
        if 0 <= (i + di) <= 70 and 0 <= (j + dj) <= 70 and grid[i + di][j + dj]:
            grid[i + di][j + dj] = False
            q.append((i + di, j + dj, steps + 1))

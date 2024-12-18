falling_bytes = '''your input here'''.splitlines()

from collections import deque

NEIGHBORS = ((-1, 0), (0, -1), (0, 1), (1, 0))

def bfs(fallen_bytes):
    grid = [[True for _ in range(71)] for _ in range(71)]
    for byte in falling_bytes[:fallen_bytes]:
        i, j = byte.split(',')
        grid[int(i)][int(j)] = False

    q = deque()

    # i, j, steps
    q.append((0, 0, 0))

    while q:
        i, j, steps = q.popleft()
        if (i == (len(grid) - 1)) and (j == (len(grid) - 1)):
            return True

        for di, dj in NEIGHBORS:
            if 0 <= (i + di) < len(grid) and 0 <= (j + dj) < len(grid) and grid[i + di][j + dj]:
                grid[i + di][j + dj] = False
                q.append((i + di, j + dj, steps + 1))

    return False

def binary_search(lower_bound, upper_bound):
    while lower_bound < upper_bound:
        m = (lower_bound + upper_bound) // 2
        if bfs(m):
            lower_bound = m + 1
        else:
            upper_bound = m

    return lower_bound

print(falling_bytes[binary_search(0, len(falling_bytes)) - 1])

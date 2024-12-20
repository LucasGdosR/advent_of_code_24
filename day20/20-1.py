racetrack = '''your input here'''.splitlines()

DIRECTIONS =            ((-1, 0), (1, 0), (0, 1), (0, -1))
TWO_PS_CHEATING_JUMPS = ((-2, 0), (2, 0), (0, 2), (0, -2))

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

def find_cheats(i, j, steps):
    return sum(1 for di, dj in TWO_PS_CHEATING_JUMPS if path.get((i+di, j+dj), 0) >= steps + 102)
    
for i, row in enumerate(racetrack):
    for j, c in enumerate(row):
        if c == 'S':
            path[(i, j)] = 0
            iterative_dfs(i, j, 0)
            print(sum(find_cheats(i, j, steps) for (i, j), steps in path.items()))
            exit()

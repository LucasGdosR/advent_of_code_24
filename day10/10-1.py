grid = [[int(c) for c in s] for s in '''89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732'''.splitlines()]

TRAILHEAD = 0
HILLTOP = 9

DIRECTIONS = ((-1, 0),
        (0, -1),    ( 0, 1),
              ( 1, 0))

def hike_uphill(i, j, e):
    if e == HILLTOP:
        hiking_trails.add((i, j))
        return
    
    for di, dj in DIRECTIONS:
        if ((0 <= (i + di) < len(grid)) and
            (0 <= (j + dj) < len(grid[0])) and
            (grid[i + di][j + dj] == (e + 1))
        ):
            hike_uphill(i + di, j + dj, e + 1)

score = 0
for i, row in enumerate(grid):
    for j, e in enumerate(row):
        if e == TRAILHEAD:
            hiking_trails = set()
            hike_uphill(i, j, TRAILHEAD)
            score += len(hiking_trails)
print(score)   

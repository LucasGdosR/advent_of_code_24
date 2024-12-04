input = '''MMMSXXMASM
MSAMXMSMSA
AMXSXMAAMM
MSAMASMSMX
XMASAMXAMM
XXAMMXXAMA
SMSMSASXSS
SAXAMASAAA
MAMMMXMMMM
MXMXAXMASX'''

grid = input.splitlines()

directions = [
    (-1, -1), (0, -1), (1, -1),
    (-1,  0),          (1,  0),
    (-1,  1), (0,  1), (1,  1)
]

def find_MAS(x, y):
    count = 0
    for dx, dy in directions:
        if (
            (0 <= (x + 3*dx) < len(grid))
            and (0 <= (y + 3*dy) < len(grid[0]))
            and (grid[x + 1*dx][y + 1*dy] == 'M')
            and (grid[x + 2*dx][y + 2*dy] == 'A')
            and (grid[x + 3*dx][y + 3*dy] == 'S')
            ):
            count += 1
    return count
    
count = 0
for i in range(len(grid)):
    for j in range(len(grid[0])):
        if grid[i][j] == 'X':
            count += find_MAS(i, j)
print(count)

count = 0
def find_cross(x, y):
    d1 = is_cross((x - 1, y - 1), (x + 1, y + 1))
    d2 = is_cross((x + 1, y - 1), (x - 1, y + 1))
    return d1 and d2

def is_cross(p1, p2):
    c1, c2 = grid[p1[0]][p1[1]], grid[p2[0]][p2[1]]
    return (c1 == 'M' and c2 == 'S') or (c1 == 'S' and c2 == 'M')

# Skip borders
for i in range(1, len(grid) - 1):
    for j in range(1, len(grid[0]) - 1):
        if grid[i][j] == 'A':
            count += find_cross(i, j)
print(count)

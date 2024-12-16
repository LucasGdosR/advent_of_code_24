input = '''AAAA
BBCD
BBCC
EEEC'''

NEIGHBORS = ((-1, 0),
       (0, -1),    (0, 1),
             ( 1, 0))

def budget_garden_fences(garden: str) -> int:
    garden = garden.splitlines()
    visited = [[False for _ in row] for row in garden]
    budget = 0
    
    def expand_region(plant: str, i: int, j: int, region: set[tuple[int, int]]) -> None:
        for di, dj in NEIGHBORS:
            if 0 <= (i + di) < len(garden) and 0 <= (j + dj) < len(garden) and garden[i + di][j + dj] == plant and not visited[i + di][j + dj]:
                visited[i + di][j + dj] = True
                region.add((i + di, j + dj))
                expand_region(plant, i + di, j + dj, region)
    
    def calculate_sides(region: set[tuple[int, int]]) -> int:
        is_corner = [[False for _ in range(len(garden[0]) + 1)] for _ in range(len(garden) + 1)]
        edge_cases = 0
        for i, j in region:
            if (i + 1, j + 1) in region and (i + 1, j) not in region and (i, j + 1) not in region:
                edge_cases += 1
            if (i + 1, j - 1) in region and (i + 1, j) not in region and (i, j -1) not in region:
                edge_cases += 1
            is_corner[i][j] = not is_corner[i][j]
            is_corner[i+1][j] = not is_corner[i+1][j]
            is_corner[i][j+1] = not is_corner[i][j+1]
            is_corner[i+1][j+1] = not is_corner[i+1][j+1]
        return sum((corner for row in is_corner for corner in row)) + 2 * edge_cases
        
    for i, row in enumerate(garden):
        for j, plant in enumerate(row):
            if not visited[i][j]:
                visited[i][j] = True
                region = {(i, j)}
                expand_region(plant, i, j, region)
                area = len(region)
                sides = calculate_sides(region)
                budget += area * sides
                
    return budget

print(budget_garden_fences(input))

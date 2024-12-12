small_example_1 = '''AAAA
BBCD
BBCC
EEEC'''

small_example_2 = '''OOOOO
OXOXO
OOOOO
OXOXO
OOOOO'''

large_example = '''RRRRIICCFF
RRRRIICCCF
VVRRRCCFFF
VVRCCCJFFF
VVVVCJJCFE
VVIVCCJJEE
VVIIICJJEE
MIIIIIJJEE
MIIISIJEEE
MMMISSJEEE'''

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
    
    def calculate_perimeter(region: set[tuple[int, int]]) -> int:
            return sum((i + di, j + dj) not in region for i, j in region for di, dj in NEIGHBORS)
        
    for i, row in enumerate(garden):
        for j, plant in enumerate(row):
            if not visited[i][j]:
                visited[i][j] = True
                region = {(i, j)}
                expand_region(plant, i, j, region)
                area = len(region)
                perimeter = calculate_perimeter(region)
                print(f'Plant: {plant}; Area: {area}; Perimeter: {perimeter}')
                budget += area * perimeter
                
    return budget

print(f'First example: {budget_garden_fences(small_example_1)}\n')
print(f'Second example: {budget_garden_fences(small_example_2)}\n')
print(f'Third example: {budget_garden_fences(large_example)}')

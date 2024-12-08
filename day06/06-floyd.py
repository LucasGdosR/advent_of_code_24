input = '''....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...'''.splitlines()

from enum import Enum

class Facing(Enum):
    UP = 0
    RIGHT = 1
    DOWN = 2
    LEFT = 3

facing_progression = [Facing.RIGHT, Facing.DOWN, Facing.LEFT, Facing.UP]

def find_guard():
    for i in range(len(input)):
        for j in range(len(input[0])):
            if input[i][j] == '^':
                return {'facing': Facing.UP, 'x': j, 'y': i }

g = find_guard()
regular_path = set()
regular_path.add((g['x'], g['y']))
starting_states = set()

def finding_regular_path():
    x = g['x'] + (g['facing'] == Facing.RIGHT) - (g['facing'] == Facing.LEFT)
    y = g['y'] + (g['facing'] == Facing.DOWN) - (g['facing'] == Facing.UP)
    
    if x == -1 or x == len(input) or y == -1 or y == len(input):
        print("Unique tiles visited: " + str(len(regular_path)))
        return False
    
    if '#' == input[y][x]:
        g['facing'] = facing_progression[g['facing'].value]
    else:
        if (x, y) not in regular_path:
            regular_path.add((x, y))
            starting_states.add((g['facing'], g['x'], g['y']))
        g['x'] = x
        g['y'] = y
    return True

while finding_regular_path():
    continue

def floyd_cycle_detection(g):
    tortoise = g.copy()
    hare = g.copy()
    while True:
        tortoise = move(tortoise)
        hare = move(move(hare))
        if hare is None:
            return False
        if tortoise == hare:
            return True

def move(g):
    if g is None:
        return None
    
    x = g['x'] + (g['facing'] == Facing.RIGHT) - (g['facing'] == Facing.LEFT)
    y = g['y'] + (g['facing'] == Facing.DOWN) - (g['facing'] == Facing.UP)
    if x == -1 or x == len(input) or y == -1 or y == len(input):
        return None
    
    if ('#' == input[y][x]) or ((x == obstacle[0]) and (y == obstacle[1])):
        g['facing'] = facing_progression[g['facing'].value]
    else:
        g['x'] = x
        g['y'] = y
    return g

loops_found = 0
for f, x, y in starting_states:
    g = {'facing': f, 'x': x, 'y': y}
    x += (f == Facing.RIGHT) - (f == Facing.LEFT)
    y += (f == Facing.DOWN ) - (f == Facing.UP)
    obstacle = (x, y)
    loops_found += floyd_cycle_detection(g)
print(loops_found)

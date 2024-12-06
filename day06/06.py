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

initial_guard = find_guard()
initial_state = set()
initial_state.add((initial_guard['facing'], initial_guard['x'], initial_guard['y']))
regular_path = set()
regular_path.add((initial_guard['x'], initial_guard['y']))

def move():
    x = g['x'] + (g['facing'] == Facing.RIGHT) - (g['facing'] == Facing.LEFT)
    y = g['y'] + (g['facing'] == Facing.DOWN) - (g['facing'] == Facing.UP)
    
    if x == -1 or x == len(input) or y == -1 or y == len(input):
        print("Unique tiles visited: " + str(len(regular_path)))
        return False
    
    if '#' == input[y][x]:
        g['facing'] = facing_progression[g['facing'].value]
    else:
        regular_path.add((x, y))
        g['x'] = x
        g['y'] = y
    return True

g = initial_guard.copy()
while move():
    continue

def test_loop():
    global loops_found
    
    x = g['x'] + (g['facing'] == Facing.RIGHT) - (g['facing'] == Facing.LEFT)
    y = g['y'] + (g['facing'] == Facing.DOWN) - (g['facing'] == Facing.UP)
    if (x == -1
     or x == len(input)
     or y == -1
     or y == len(input)):
        return False
    
    if ('#' == input[y][x]) or ((x == obstacle[0]) and (y == obstacle[1])):
        g['facing'] = facing_progression[g['facing'].value]
    else:
        g['x'] = x
        g['y'] = y
    state = (g['facing'], g['x'], g['y'])
    if state in visited:
        loops_found += 1
        return False
    visited.add(state)
    return True

loops_found = 0
for obstacle in regular_path:
    visited = initial_state.copy()
    g = initial_guard.copy()
    while test_loop():
        continue
print(loops_found)

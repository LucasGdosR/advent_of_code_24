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

def move():
    global keep_going
    global loops_found
    
    x = guard['x'] + (guard['facing'] == Facing.RIGHT) - (guard['facing'] == Facing.LEFT)
    y = guard['y'] + (guard['facing'] == Facing.DOWN) - (guard['facing'] == Facing.UP)
    if (x == -1
     or x == len(input)
     or y == -1
     or y == len(input)):
        keep_going = False
        return
    
    if ('#' == input[y][x]) or ((x == fake_obstacle[0]) and (y == fake_obstacle[1])):
        guard['facing'] = facing_progression[guard['facing'].value]
    else:
        guard['x'] = x
        guard['y'] = y
    state = (guard['facing'], guard['x'], guard['y'])
    if state in visited:
        keep_going = False
        loops_found += 1
        return
    visited.add(state)

loops_found = 0
for i in range(len(input)):
    for j in range(len(input)):
        fake_obstacle = (i, j)
        keep_going = True
        visited = initial_state.copy()
        guard = initial_guard.copy()
        while keep_going:
            move()
print(loops_found)

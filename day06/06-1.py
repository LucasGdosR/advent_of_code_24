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

guard = find_guard()

visited = set()
visited.add((guard['x'], guard['y']))

def move():
    x = guard['x'] + (guard['facing'] == Facing.RIGHT) - (guard['facing'] == Facing.LEFT)
    y = guard['y'] + (guard['facing'] == Facing.DOWN) - (guard['facing'] == Facing.UP)
    
    if x == -1 or x == len(input) or y == -1 or y == len(input):
        print(len(visited))
        exit()
    
    if '#' == input[y][x]:
        guard['facing'] = facing_progression[guard['facing'].value]
    else:
        visited.add((x, y))
        guard['x'] = x
        guard['y'] = y
    
while True:
    move()

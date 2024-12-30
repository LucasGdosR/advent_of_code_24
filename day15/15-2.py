old_warehouse = '''##########
#..O..O.O#
#......O.#
#.OO..O.O#
#..O@..O.#
#O#..O...#
#O..O..O.#
#.OO.O.OO#
#....O...#
##########'''.splitlines()

moves = '''<vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
<<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
>^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
<><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^'''

from collections import deque

new_warehouse = [[] for _ in range(len(old_warehouse))]
for i, row in enumerate(old_warehouse):
    for j, c in enumerate(row):
        if c == 'O':
            new_warehouse[i].append('[')
            new_warehouse[i].append(']')
        elif c == '@':
            robot = [i, 2 * j]
            new_warehouse[i].append('@')
            new_warehouse[i].append('.')
        else:
            new_warehouse[i].append(c)
            new_warehouse[i].append(c)

def try_to_move(m):
    di, dj = get_directions(m)
    
    dst_i = robot[0] + di
    dst_j = robot[1] + dj
    dst = new_warehouse[dst_i][dst_j]
    
    if dst == '.':
        new_warehouse[robot[0]][robot[1]] = '.'
        new_warehouse[dst_i][dst_j] = '@'
        robot[0], robot[1] = dst_i, dst_j
        
    elif dst in '[]':
        if dj:
            try_to_move_large_box_horizontally(dj)
        elif can_move_vertically(dst_i, dst_j, di):
            offset = dst == ']'
            move_large_box_vertically(dst_i, dst_j - offset, di)
            new_warehouse[dst_i][dst_j] = '@'
            new_warehouse[dst_i][(dst_j + 1) if dst == '[' else (dst_j - 1)] = '.'
            new_warehouse[robot[0]][robot[1]] = '.'
            robot[0] = dst_i  

def try_to_move_large_box_horizontally(dj):
    i = robot[0]
    dst_j = robot[1] + dj
    
    while True:
        dst = new_warehouse[i][dst_j]
        if dst == '#':
            return
        elif dst == '.':
            break
        dst_j += 2 * dj
    
    open_or_close = '[' if dj == 1 else ']'
    close_or_open = ']' if dj == 1 else '['
    
    for j in range(robot[1] + 2 * dj, dst_j, 2 * dj):
        new_warehouse[i][j] = open_or_close
        new_warehouse[i][j + dj] = close_or_open
        
    new_warehouse[i][robot[1]] = '.'
    new_warehouse[i][robot[1] + dj] = '@'
    robot[1] += dj

def can_move_vertically(i, j, di):
    dst = new_warehouse[i][j]
    if dst == '.':
        return True
    if dst == '#':
        return False
    offset = new_warehouse[i][j] == ']'
    i += di
    return can_move_vertically(i, j - offset, di) and can_move_vertically(i, j - offset + 1, di)

def move_large_box_vertically(i, left_j, di):
    next_i = i + di
    right_j = left_j + 1
    
    if new_warehouse[next_i][left_j] == '[':
        move_large_box_vertically(next_i, left_j, di)
    else:
        if new_warehouse[next_i][left_j] == ']':
            move_large_box_vertically(next_i, left_j - 1, di)
        if new_warehouse[next_i][right_j] == '[':
            move_large_box_vertically(next_i, right_j, di)
    
    new_warehouse[next_i][left_j] = '['
    new_warehouse[next_i][right_j] = ']'
    new_warehouse[i][left_j] = '.'
    new_warehouse[i][right_j] = '.'

def get_directions(m):
    if m == '<':
        return 0, -1
    elif m == '>':
        return 0, 1
    elif m == '^':
        return -1, 0
    elif m == 'v':
        return 1, 0
    else:
        return 0, 0

for m in moves:
    try_to_move(m)

gps = 0
for i, row in enumerate(new_warehouse):
    for j, c in enumerate(row):
        if c == '[':
            gps += 100 * i + j
  
print(gps)

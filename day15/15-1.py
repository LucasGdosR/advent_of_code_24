warehouse = '''##########
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

warehouse = [[c for c in s] for s in warehouse]
for i, row in enumerate(warehouse):
    for j, c in enumerate(row):
        if c == '@':
            robot = [i, j]
            break

def try_to_move(di, dj):
    dst_i = robot[0] + di
    dst_j = robot[1] + dj
    dst = warehouse[dst_i][dst_j]
    if dst == '.':
        warehouse[robot[0]][robot[1]] = '.'
        warehouse[dst_i][dst_j] = '@'
        robot[0] += di
        robot[1] += dj
    elif dst == 'O':
        while True:
            dst_i += di
            dst_j += dj
            dst = warehouse[dst_i][dst_j]
            if dst == '#':
                return
            elif dst == '.':
                warehouse[dst_i][dst_j] = 'O'
                break
        warehouse[robot[0]][robot[1]] = '.'
        warehouse[robot[0] + di][robot[1] + dj] = '@'
        robot[0] += di
        robot[1] += dj

for m in moves:
    if m == '<':
        di, dj = 0, -1
    elif m == '>':
        di, dj = 0, 1
    elif m == '^':
        di, dj = -1, 0
    elif m == 'v':
        di, dj = 1, 0
    else:
        di, dj = 0, 0
    try_to_move(di, dj)

gps = 0
for i, row in enumerate(warehouse):
    for j, c in enumerate(row):
        if c == 'O':
            gps += 100 * i + j
  
print(gps)

new_warehouse = [[] for _ in range(len(old_warehouse))]
for i, row in enumerate(old_warehouse):
    for j, c in enumerate(row):
        if c == '#':
            new_warehouse[i].append('#')
            new_warehouse[i].append('#')
        elif c == '.':
            new_warehouse[i].append('.')
            new_warehouse[i].append('.')
        elif c == 'O':
            new_warehouse[i].append('[')
            new_warehouse[i].append(']')
        else:
            robot = [i, 2 * j]
            new_warehouse[i].append('@')
            new_warehouse[i].append('.')
            
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

def try_to_move(m):
    global new_warehouse
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
            temp_warehouse = [[c for c in s] for s in new_warehouse]
            move_large_box_vertically(dst_i, dst_j - offset, di, temp_warehouse)
            new_warehouse = temp_warehouse
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

# .... -> .[].
# .[]. -> [][]
# [][] -> .[].
# .[]. -> ..@.
# ..@. -> ....
def move_large_box_vertically(i, left_j, di, temp_warehouse):
    # i and left_j are the coordinates of this box
    # Put this box on the next line
    next_i = i + di
    right_j = left_j + 1
    temp_warehouse[next_i][left_j] = '['
    temp_warehouse[next_i][right_j] = ']'
    
    # If the next line had boxes, move them and clean them up
    old_left, old_right = new_warehouse[next_i][left_j], new_warehouse[next_i][right_j]
    
    if old_left == '[':
        move_large_box_vertically(next_i, left_j, di, temp_warehouse)
    else:
        if old_left == ']':
            move_large_box_vertically(next_i, left_j - 1, di, temp_warehouse)
            temp_warehouse[next_i][left_j - 1] = '.'
        if old_right == '[':
            move_large_box_vertically(next_i, right_j, di, temp_warehouse)
            temp_warehouse[next_i][right_j + 1] = '.'

for m in moves:
    try_to_move(m)

for row in new_warehouse:
    for c in row:
        print(c, end='')
    print()

gps = 0
for i, row in enumerate(new_warehouse):
    for j, c in enumerate(row):
        if c == '[':
            gps += 100 * i + j
  
print(gps) # 1404922 too low

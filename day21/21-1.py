codes = '''029A
980A
179A
456A
379A'''.splitlines()

from collections import deque

key_to_coordinates = { '7': (0, 0), '8': (0, 1), '9': (0, 2),
                       '4': (1, 0), '5': (1, 1), '6': (1, 2),
                       '1': (2, 0), '2': (2, 1), '3': (2, 2),
                                    '0': (3, 1), 'A': (3, 2) }

coordinates_to_key = ((None, '^', 'A'),
                      ( '<', 'v', '>'))

'''
    Pressing keys at will modifies i1 and j1 directly, or confirms, indirectly modifying the rest
'''
def move_left(i1, j1, i2, j2, i3, j3):
    return j1 == 2 or (i1 == 1 and j1 == 1), (i1, j1 - 1, i2, j2, i3, j3)

def move_right(i1, j1, i2, j2, i3, j3):
    return j1 < 2, (i1, j1 + 1, i2, j2, i3, j3)

def move_up(i1, j1, i2, j2, i3, j3):
    return i1 == 1 and j1 != 0, (0, j1, i2, j2, i3, j3)

def move_down(i1, j1, i2, j2, i3, j3):
    return i1 == 0, (1, j1, i2, j2, i3, j3)

def confirm(i1, j1, i2, j2, i3, j3):
    key = coordinates_to_key[i1][j1]
    return _action[key](i1, j1, i2, j2, i3, j3)

'''
    First level of indirection. One robot activates the other, directly modifying i2 and j2,
    or indirectly modifying the rest.
'''
def _move_left(i1, j1, i2, j2, i3, j3):
    return j2 == 2 or (i2 == 1 and j2 == 1), (i1, j1, i2, j2 - 1, i3, j3)

def _move_right(i1, j1, i2, j2, i3, j3):
    return j2 < 2, (i1, j1, i2, j2 + 1, i3, j3)

def _move_up(i1, j1, i2, j2, i3, j3):
    return i2 == 1 and j2 != 0, (i1, j1, 0, j2, i3, j3)

def _move_down(i1, j1, i2, j2, i3, j3):
    return i2 == 0, (i1, j1, 1, j2, i3, j3)

def _confirm(i1, j1, i2, j2, i3, j3):
    key = coordinates_to_key[i2][j2]
    return __action[key](i1, j1, i2, j2, i3, j3)

'''
    Final level of indirection. The robot presses a button that moves the robot that's typing the code.
'''
def __move_left(i1, j1, i2, j2, i3, j3):
    return j3 == 2 or (i3 <= 2 and j3 == 1), (i1, j1, i2, j2, i3, j3 - 1)

def __move_right(i1, j1, i2, j2, i3, j3):
    return j3 < 2, (i1, j1, i2, j2, i3, j3 + 1)

def __move_up(i1, j1, i2, j2, i3, j3):
    return i3 >= 1, (i1, j1, i2, j2, i3 - 1, j3)

def __move_down(i1, j1, i2, j2, i3, j3):
    return i3 <= 1 or (j3 >= 1 and i3 == 2), (i1, j1, i2, j2, i3 + 1, j3)

def __confirm(_i1, _j1, _i2, _j2, _i3, _j3):
    # The 'A' button is pressed between steps of the search
    return False, None
    
actions = (('<', move_left), ('>', move_right), ('^', move_up), ('v', move_down), ('A', confirm))
_action = { '<': _move_left,  '>': _move_right,  '^': _move_up,  'v': _move_down,  'A': _confirm }
__action = { '<': __move_left,  '>': __move_right,  '^': __move_up,  'v': __move_down,  'A': __confirm }

def bfs(src: str, dst: str) -> list[str]:
    q = deque()
    i3, j3 = key_to_coordinates[src]
    di, dj = key_to_coordinates[dst]
           #  i1  j1  i2  j2  i3  j3  commands
    q.append(( 0,  2,  0,  2, i3, j3, []))
    visited = set()
    visited.add((0, 2, 0, 2, i3, j3))
    while q:
        i1, j1, i2, j2, i3, j3, commands = q.popleft()
        for command, action in actions:
            is_valid, new_state = action(i1, j1, i2, j2, i3, j3)
            if is_valid and new_state not in visited:
                new_commands = commands.copy()
                new_commands.append(command)
                if new_state == (0, 2, 0, 2, di, dj):
                    return new_commands
                visited.add(new_state)
                q.append((*new_state, new_commands))            

total = 0
for code in codes:
    commands = []
    commands.extend(bfs('A', code[0]))
    commands.append('A')
    commands.extend(bfs(code[0], code[1]))
    commands.append('A')
    commands.extend(bfs(code[1], code[2]))
    commands.append('A')
    commands.extend(bfs(code[2], code[3]))
    commands.append('A')
    print(f"Code: {code}. Length: {len(commands)}. Commands: {''.join(commands)}")
    total += int(code[:3]) * len(commands)
print(total)

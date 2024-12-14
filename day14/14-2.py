initial_positions_and_velocities = '''your input here'''.splitlines()

WIDTH = 101
HEIGHT = 103

robots = []
for robot in initial_positions_and_velocities:
    p, v = robot.split(' ')
    px, py = p[2:].split(',')
    vx, vy = v[2:].split(',')
    robots.append((int(px), int(vx), int(py), int(vy)))

def look_for_tree(simulation_time):
    viz = [['.' for _ in range(WIDTH)] for _ in range(HEIGHT)]
    positions = (((px + vx * simulation_time) % WIDTH, (py + vy * simulation_time) % HEIGHT) for px, vx, py, vy in robots)
    for px, py in positions:
        viz[py][px] = '#'
    
    print(f'__________________________\nTIME: {simulation_time}\n__________________________')
    for row in viz:
        for c in row:
            print(c, end='')
        print()
    
'''
Look for two occurrences:
    a drawing with mostly horizontal lines,
    another with mostly vertical lines.
    
    Example: mostly horizontal at 70, mostly vertical at 10.
'''
for i in range (0, HEIGHT):
    look_for_tree(i)

mostly_horizontal = int(input()) # 70 with my input
'''
The horizontal pattern repeats every HEIGHT iterations,
the vertical pattern repeats every WIDTH iterations.
Find the number where they overlap.
mostly_vertical + i * WIDTH == mostly_horizontal + j * HEIGHT == answer

Example:
(10 + 73 * 101) == (70 + 71 * 103)
== 7383
'''
for i in range(mostly_horizontal, HEIGHT * WIDTH, HEIGHT):
    look_for_tree(i)

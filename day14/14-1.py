initial_positions_and_velocities = '''your input here'''.splitlines()

WIDTH = 101
HEIGHT = 103
SIMULATION_TIME = 100

final_positions = []
for robot in initial_positions_and_velocities:
    p, v = robot.split(' ')
    px, py = p[2:].split(',')
    vx, vy = v[2:].split(',')
    final_positions.append([(int(px) + int(vx) * SIMULATION_TIME) % WIDTH, (int(py) + int(vy) * SIMULATION_TIME) % HEIGHT])

robots_per_quadrant = [0, 0, 0, 0]
for x, y in final_positions:
    delim_x, delim_y = WIDTH // 2, HEIGHT // 2
    if x == delim_x or y == delim_y:
        continue
    elif x < delim_x and y < delim_y:
        robots_per_quadrant[0] += 1
    elif x < delim_x and y > delim_y:
        robots_per_quadrant[1] += 1
    elif x > delim_x and y < delim_y:
        robots_per_quadrant[2] += 1
    else:
        robots_per_quadrant[3] += 1

from math import prod
print(prod(robots_per_quadrant))

import numpy as np
import re

def parse_file(filename):
    with open(filename, 'r') as f:
        content = f.read()
  
    return [
        {
            line[7] if len(line.split(': ')[0]) == 8 else 'P': {'X': int(x), 'Y': int(y)}
            for line in entry.splitlines()
            for x, y in [re.findall(r'\d+', line.split(': ')[1])]
        }
        for entry in content.split('\n\n')
    ]
    
entries = parse_file('13-input')
for entry in entries:
    for k, v in entry.items():
        if k == 'P':
            entry['P']['X'] += 10000000000000
            entry['P']['Y'] += 10000000000000
        

A_COST, B_COST = 3, 1

def calc_cost(px, py, ax, ay, bx, by) -> int:
    '''
    ax * a_pressed + bx * b_pressed = px
    ay * a_pressed + by * b_pressed = py
    '''
    a = np.array([[ax, bx], [ay, by]])
    b = np.array([px, py])
    a_pressed, b_pressed = np.linalg.solve(a, b)
    if (is_integer(a_pressed) and is_integer(b_pressed)):
        return round(a_pressed) * A_COST + round(b_pressed) * B_COST
    return 0

def is_integer(num):
    return abs(num - round(num)) < 0.001

print(
    sum(
        (
            calc_cost(
                entry['P']['X'], entry['P']['Y'],
                entry['A']['X'], entry['A']['Y'],
                entry['B']['X'], entry['B']['Y']
            ) for entry in entries
        )
    )
)

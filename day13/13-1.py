import re
from functools import lru_cache

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

A_COST, B_COST = 3, 1

def calc_cost(px, py, ax, ay, bx, by) -> int:
    @lru_cache(maxsize=None)
    def calc_cost_this_A_this_B(px, py, a_pressed=0, b_pressed=0):
        # Base cases
        # Success
        if px == 0 and py == 0:
            return a_pressed * A_COST + b_pressed * B_COST
        # Failure: return 0 means spend 0 tokens trying to get this prize, because it's impossible
        if px < 0 or py < 0 or a_pressed > 100 or b_pressed > 100:
            return 0
        
        # Get the minimum, but skip zeroes
        cost_of_pressing_A = calc_cost_this_A_this_B(px - ax, py - ay, a_pressed + 1, b_pressed)
        cost_of_pressing_B = calc_cost_this_A_this_B(px - bx, py - by, a_pressed, b_pressed + 1)
        
        return min(cost_of_pressing_A, cost_of_pressing_B) if cost_of_pressing_A and cost_of_pressing_B else cost_of_pressing_A if cost_of_pressing_A else cost_of_pressing_B
    
    return calc_cost_this_A_this_B(px, py)

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

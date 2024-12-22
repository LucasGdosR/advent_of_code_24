input = '''1
10
100
2024'''.splitlines()

def random(seed):
    seed = (seed ^ (seed << 6)) % 16777216
    seed = (seed ^ (seed >> 5)) % 16777216
    return (seed ^ (seed << 11)) % 16777216
    
total = 0
for seed in input:
    seed = int(seed)
    for i in range(2000):
        seed = random(seed)
    total += seed
print(total)

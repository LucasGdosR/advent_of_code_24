input = '''1
10
100
2024'''.splitlines()

def random(seed):
    seed = (seed ^ (seed << 6)) & 0xFFFFFF
    seed = (seed ^ (seed >> 5)) & 0xFFFFFF
    return (seed ^ (seed << 11)) & 0xFFFFFF
    
total = 0
for seed in input:
    seed = int(seed)
    for i in range(2000):
        seed = random(seed)
    total += seed
print(total)

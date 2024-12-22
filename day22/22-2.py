input = '''1
2
3
2024'''.splitlines()

def random(seed):
    seed = (seed ^ (seed << 6)) & 0xFFFFFF
    seed = (seed ^ (seed >> 5)) & 0xFFFFFF
    return (seed ^ (seed << 11)) & 0xFFFFFF

from collections import defaultdict

profits = defaultdict(lambda: 0)

for seed in input:
    seed = int(seed)
    price_changes = []
    seen_sequences = set()
    for _ in range(2000):
        prev_price = seed % 10
        seed = random(seed)
        curr_price = seed % 10
        price_changes.append(curr_price - prev_price)
        if len(price_changes) >= 4:
            seq = tuple(price_changes[-4:])
            if seq not in seen_sequences:
                seen_sequences.add(seq)
                profits[seq] += curr_price

print(max(profits.items(), key=lambda i: i[1]))

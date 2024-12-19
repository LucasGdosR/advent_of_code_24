available_towels = '''r, wr, b, g, bwu, rb, gb, br'''.split(', ')

target_patterns = '''brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb'''.splitlines()

memo = {'': 1}

def count_arrangements(pattern):
    if pattern not in memo:
        memo[pattern] = sum(count_arrangements(pattern[len(towel):]) for towel in available_towels if pattern.startswith(towel))
    return memo[pattern]
print(sum(count_arrangements(pattern) for pattern in target_patterns))

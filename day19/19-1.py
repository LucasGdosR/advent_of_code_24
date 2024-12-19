available_towels = '''r, wr, b, g, bwu, rb, gb, br'''.split(', ')

target_patterns = '''brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb'''.splitlines()

memo = {'': True}

def can_form(pattern):
    if pattern not in memo:
        memo[pattern] = any(can_form(pattern[len(towel):]) for towel in available_towels if pattern.startswith(towel))
    return memo[pattern]
print(sum(can_form(pattern) for pattern in target_patterns))

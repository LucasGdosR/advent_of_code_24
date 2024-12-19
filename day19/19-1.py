available_towels = set('''r, wr, b, g, bwu, rb, gb, br'''.split(', '))

target_patterns = '''brwrr
bggr
gbbr
rrbgbr
ubwu
bwurrg
brgr
bbrgwb'''.splitlines()

memo = {}

def can_form(pattern):
    if pattern in memo:
        return memo[pattern]
    if pattern in available_towels:
        return True
    if len(pattern) == 1:
        return False
    
    for i in range(1, len(pattern)):
        if can_form(pattern[i:]) and can_form(pattern[:i]):
            memo[pattern] = True
            return True
    
    memo[pattern] = False
    return False

print(sum((can_form(pattern) for pattern in target_patterns)))

# Paste your actual input in this variable
input = '''7 6 4 2 1
1 2 7 8 9
9 7 6 2 1
1 3 2 4 5
8 6 4 4 1
1 3 6 7 9'''

reports = input.split('\n')
safe_reports = 0

def is_safe(report):
    levels = list(map(int, report.split(' ')))
    increasing = levels[0] < levels[1]
    for prev, curr in zip(levels, levels[1:]):
        if ((prev < curr) != increasing) or (abs(prev - curr) > 3) or prev == curr:
            return False
    return True

def generate_combinations(levels):
    combinations = []
    for i in range(len(levels)):
        combination = levels[:i] + levels[i+1:]
        combinations.append(combination)
    return combinations

for report in reports:
    levels = list(map(int, report.split(' ')))
    for combination in generate_combinations(levels):
        if is_safe(' '.join(map(str, combination))):
            safe_reports += 1
            
print(safe_reports)

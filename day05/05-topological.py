from collections import defaultdict

# Paste the first part of your input here
page_ordering_rules = '''47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13'''.splitlines()

adjacency_list = defaultdict(list)
for rule in page_ordering_rules:
    src, dst = rule.split('|')
    adjacency_list[int(src)].append(int(dst))

topological_order = []
visited = set()
def dfs(node):
    if node in visited:
        return
    visited.add(node)
    for neighbor in adjacency_list[node]:
        dfs(neighbor)
    topological_order.append(node)

for node in adjacency_list.copy().keys():
    dfs(node)

topological_order.reverse()

# Paste the second part of your input here
updates = '''75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47'''.splitlines()

valid_sum = 0
incorrect_sum = 0

updates = [list(map(int, update.split(','))) for update in updates]

def is_valid(u: list[int]):
    i = 0
    for page in topological_order:
        i += page == u[i]
        if i == len(u):
            return True
    return False
    
def fix(u: list[int]):
    correct_order = []
    for page in topological_order:
        if page in u:
            correct_order.append(page)
        if len(u) == len(correct_order):
            return correct_order[len(u) // 2]
    
for update in updates:
    if is_valid(update):
        valid_sum += update[len(update) // 2]
    else:
        incorrect_sum += fix(update)
    
print(valid_sum, incorrect_sum)

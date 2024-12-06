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
    dst, src = rule.split('|')
    adjacency_list[int(src)].append(int(dst))

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
    visited = set()
    for page in u:
        visited.add(page)
        for d in adjacency_list[page]:
            if d not in visited and d in u:
                return False
    return True

def fix(u: list[int]):
    correct_order = []
    while len(correct_order) != len(u):
        for page in u:
            add_this_page = True
            for d in adjacency_list[page]:
                if d not in correct_order and d in u:
                    add_this_page = False
            if add_this_page and page not in correct_order:
                correct_order.append(page)
    return correct_order[len(u) // 2]
    
for update in updates:
    if is_valid(update):
        valid_sum += update[len(update) // 2]
    else:
        incorrect_sum += fix(update)
    
print(valid_sum, incorrect_sum)

initial_rules = '''x00 AND y00 -> z05
x01 AND y01 -> z02
x02 AND y02 -> z01
x03 AND y03 -> z03
x04 AND y04 -> z04
x05 AND y05 -> z00'''.splitlines()

edges = []
for rule in initial_rules:
    i1, op, i2, _, o = rule.split()
    edges.extend(((i1, o, op), (i2, o, op)))

# Save to a dot file -> python3 24-2.py > graph.dot
print('digraph G {')
for e in edges:
    print(f'\t{e[0]} -> {e[1]} [label="{op}"];')
print('}', end='')
# Create visualization with
# dot -Tpng graph.dot -o graph.png

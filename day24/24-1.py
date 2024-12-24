initial_state = '''x00: 1
x01: 0
x02: 1
x03: 1
x04: 0
y00: 1
y01: 1
y02: 1
y03: 1
y04: 1'''.splitlines()

initial_rules = '''ntg XOR fgs -> mjb
y02 OR x01 -> tnw
kwq OR kpj -> z05
x00 OR x03 -> fst
tgd XOR rvg -> z01
vdt OR tnw -> bfw
bfw AND frj -> z10
ffh OR nrd -> bqk
y00 AND y03 -> djm
y03 OR y00 -> psh
bqk OR frj -> z08
tnw OR fst -> frj
gnj AND tgd -> z11
bfw XOR mjb -> z00
x03 OR x00 -> vdt
gnj AND wpb -> z02
x04 AND y00 -> kjc
djm OR pbm -> qhw
nrd AND vdt -> hwm
kjc AND fst -> rvg
y04 OR y02 -> fgs
y01 AND x02 -> pbm
ntg OR kjc -> kwq
psh XOR fgs -> tgd
qhw XOR tgd -> z09
pbm OR djm -> kpj
x03 XOR y03 -> ffh
x00 XOR y04 -> ntg
bfw OR bqk -> z06
nrd XOR fgs -> wpb
frj XOR qhw -> z04
bqk OR frj -> z07
y03 OR x01 -> nrd
hwm AND bqk -> z03
tgd XOR rvg -> z12
tnw OR pbm -> gnj'''.splitlines()

from collections import defaultdict

state = {}
for s in initial_state:
    state[s[:3]] = int(s[5])

rules = defaultdict(list)
for r in initial_rules:
    i1, op, i2, _, o = r.split()
    op = (lambda a, b: a & b) if (op == 'AND') else (lambda a, b: a | b) if (op == 'OR') else (lambda a, b: a ^ b)
    rules[(i1, i2)].append((o, op))

while rules:
    for (i1, i2), os_and_ops in rules.copy().items():
        if i1 in state and i2 in state:
            del rules[(i1, i2)]
            s1, s2 = state[i1], state[i2]
            for o, op in os_and_ops:
                state[o] = op(s1, s2)

total = 0
for s in state:
    if s[0] == 'z':
        total |= state[s] << int(s[1:])

print(total)

edges = '''kh-tc
qp-kh
de-cg
ka-co
yn-aq
qp-ub
cg-tb
vc-aq
tb-ka
wh-tc
yn-cg
kh-ub
ta-co
de-co
tc-td
tb-wq
wh-td
ta-ka
td-qp
aq-cg
wq-ub
ub-vc
de-ta
wq-aq
wq-vc
wh-yn
ka-de
kh-ta
co-tc
wh-qp
tb-vc
td-yn'''.splitlines()

from collections import defaultdict

adj_lists = defaultdict(set)
for e in edges:
    n1, n2 = e.split('-')
    adj_lists[n1].add(n2)
    adj_lists[n2].add(n1)

three_sets = set()
for node in adj_lists:
    if node[0] == 't':
        t_neighbors = adj_lists[node]
        for n in t_neighbors:
            final_computers = t_neighbors.intersection(adj_lists[n])
            for third_computer in final_computers:
                three_sets.add(tuple(sorted((node, n, third_computer))))
print(len(three_sets))

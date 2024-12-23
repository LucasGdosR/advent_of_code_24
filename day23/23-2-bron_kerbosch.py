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

def build_graph(edges):
    graph = defaultdict(set)
    for e in edges:
        v1, v2 = e.split('-')
        graph[v1].add(v2)
        graph[v2].add(v1)
    return graph

def find_largest_clique(edges):
    graph = build_graph(edges)
    
    def get_pivot(p, x):
        """Choose vertex with most connections to P as pivot"""
        pivot = None
        max_connections = -1
        for v in p.union(x):
            connections = len(p.intersection(graph[v]))
            if connections > max_connections:
                max_connections = connections
                pivot = v
        return pivot
    
    def bron_kerbosch(r, p, x, max_clique):
        """
        r: current clique being built
        p: vertices that could extend current clique
        x: vertices already processed
        max_clique: largest clique found so far
        """
        # If no more vertices to process and no vertices already seen
        if not p and not x:
            if len(r) > len(max_clique[0]):
                max_clique[0] = r.copy()
            return
        
        # Choose pivot
        pivot = get_pivot(p, x)
        # Process vertices in P that aren't connected to pivot
        for v in p - graph[pivot]:
            # New sets for recursive call
            new_r = r | {v}
            new_p = p & graph[v]
            new_x = x & graph[v]
            # Recursive call
            bron_kerbosch(new_r, new_p, new_x, max_clique)
            # Move vertex v from P to X
            p = p - {v}
            x = x | {v}
    
    # Initialize sets
    all_vertices = set(graph.keys())
    max_clique = [set()]  # Using list to allow modification in recursive function
    
    # Start algorithm
    bron_kerbosch(set(), all_vertices, set(), max_clique)
    
    return sorted(max_clique[0])

result = find_largest_clique(edges)
print("Largest clique:", ','.join(result))

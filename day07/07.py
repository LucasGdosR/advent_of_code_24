input = '''190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20'''.splitlines()

from math import log10
# Check the commit history for a more readable version
def can_combine(v, os, acc):
    return v == acc if not os else (can_combine(v, os[1:], acc + os[0]) or can_combine(v, os[1:], acc * os[0]) or can_combine(v, os[1:], acc * 10 ** (int(log10(os[0])) + 1) + os[0]))

print(sum((test_value for test_value, operands in {int(test_value): [int(o) for o in operands.split(' ')] for test_value, operands in (eq.split(': ') for eq in input)}.items() if can_combine(test_value, operands, 0))))

import re

# Paste your actual input in this variable
corrupted_data = '''xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))'''

pattern = r"(?P<mul>mul\((\d{1,3}),(\d{1,3})\))|(?P<do>do\(\))|(?P<dont>don't\(\))"
matches = re.finditer(pattern, corrupted_data)
do = True
total = 0
for match in matches:
    if do and match.group('mul'):
        total += int(match.group(2)) * int(match.group(3))
    elif match.group('do'):
        do = True
    elif match.group('dont'):
        do = False
print(total)

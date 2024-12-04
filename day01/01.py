# Paste your actual input in this variable
location_ids = [
    3,   4,
    4,   3,
    2,   5,
    1,   3,
    3,   9,
    3,   3
]

first_list = [location_ids[i] for i in range(0, len(location_ids), 2)]
second_list = [location_ids[i] for i in range(1, len(location_ids), 2)]

first_list.sort()
second_list.sort()

distance = 0
for i in range(len(first_list)):
    distance += abs(first_list[i] - second_list[i])
print(distance)

from collections import Counter
second_list_multiplier = Counter(second_list)
similarity = 0
for id in first_list:
    similarity += id * second_list_multiplier.get(id, 0)
print(similarity)

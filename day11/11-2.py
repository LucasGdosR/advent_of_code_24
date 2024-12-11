from math import log10

# Dynamic programming cache
# Keys: (stone_num, n_blinks)
# Values: stone_count
stones_after_n_blinks = {}

def blink_n_times(stone, n_blinks):
    # Look in the cache
    if (stone, n_blinks) in stones_after_n_blinks:
        return stones_after_n_blinks[(stone, n_blinks)]
    
    # Base case
    if n_blinks == 1:
        return 1 if stone == 0 else 1 if (int(log10(stone)) + 1) & 1 else 2
    
    # Recurse
    if stone == 0:
        stone_count = blink_n_times(1, n_blinks - 1)
    else:
        num_of_digits = int(log10(stone)) + 1
        if num_of_digits & 1:
            stone_count = blink_n_times(2024 * stone, n_blinks - 1)
        else:
            children = divmod(stone, pow(10, num_of_digits >> 1))
            stone_count = sum((blink_n_times(child, n_blinks - 1) for child in children))

    stones_after_n_blinks[(stone, n_blinks)] = stone_count
    return stone_count

input_stones = [125, 17]
stone_count = 0
for stone in input_stones:
    stone_count += blink_n_times(stone, 75)
print(stone_count)

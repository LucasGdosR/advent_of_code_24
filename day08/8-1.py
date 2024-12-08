input = '''............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............'''.splitlines()

antenna_freqs = {c for s in input for c in s}
antenna_freqs.remove('.')

antinodes = [[False for _ in range(len(input[0]))] for _ in range(len(input))]

for f in antenna_freqs:
    antenna_positions = [(i, j) for i, s in enumerate(input) for j, c in enumerate(s) if c == f]
    for i, p1 in enumerate(antenna_positions):
        for p2 in antenna_positions[i + 1:]:
            small_x = p1 if p1[1] < p2[1] else p2
            big_x = p1 if p1[1] >= p2[1] else p2

            delta_x = big_x[1] - small_x[1]
            delta_y = big_x[0] - small_x[0]

            an1x = small_x[1] - delta_x
            an1y = small_x[0] - delta_y

            an2x = big_x[1] + delta_x
            an2y = big_x[0] + delta_y

            if 0 <= an1x < len(antinodes) and 0 <= an1y < len(antinodes):
                antinodes[an1y][an1x] = True
            if 0 <= an2x < len(antinodes) and 0 <= an2y < len(antinodes):
                antinodes[an2y][an2x] = True

print(sum(sum(an) for an in antinodes))

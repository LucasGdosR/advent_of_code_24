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

            if (delta_x == 0) or (delta_y == 0):
                if delta_x == 0:
                    x = p1[1]
                    for i in range(antinodes):
                        antinodes[i][x] = True
                if delta_y == 0:
                    y = p1[0]
                    for i in range(antinodes):
                        antinodes[y][i] = True
                break

            if delta_x == delta_y:
                delta_x = 1
                delta_y = 1

            y, x = small_x

            while 0 <= x < len(antinodes) and 0 <= y < len(antinodes):
                antinodes[y][x] = True
                x -= delta_x
                y -= delta_y
            
            y, x = small_x
            while 0 <= x < len(antinodes) and 0 <= y < len(antinodes):
                antinodes[y][x] = True
                x += delta_x
                y += delta_y

print(sum(sum(an) for an in antinodes))

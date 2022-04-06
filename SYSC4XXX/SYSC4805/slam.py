import math

# The side length of the scan map in pixels, so a size of 10 gives a 10x10 map.
SCAN_MAP_SIZE = 2000


# Also note that xpos and ypos may not correspond to the same X and Y positions of the map.
# Pretty sure we'll want all values to be positive although we could take advantage of negative indexing.
# Angle should be a value between [0, 360), while distance is an integer multiple of the precision (1 cm).
# The angle is from the axis that the vehicle started its path.

# Pixels have an accumulator value for the number of hits/misses, and the current count of hits.
# Probability of an obstacle is a simple # scans / # hits, with both values cut in half every 10000 scans.


class LocalScan:
    # Stores a list of scans taken from a stationary position, with each scan having a specific angle and distance.
    scan_tuple_list = list()
    xpos: float
    ypos: float

    def add_scan(self, angle: float, distance: int):
        self.scan_tuple_list.append((angle, distance))


def get_pos_from_map(scan: LocalScan):
    # This might be a bit too much for our poor script, might delay the sim to do all this math :(
    pass


def get_pixels_from_scan(xpos: float, ypos: float, scan: tuple) -> list:
    # The pixel [0][0] should have a sim position value of (0,0), with +-0.5 value.
    # The pixel [0][1] would correspond to values of ((-0.5, 0.5)(0.5, 1.5))
    # If the given beam intersects a pixel, that pixel will be considered as possibly clear.
    # The last value in the list is the hit, all others are misses.
    # This uses my shitty interpretation of the algorithm described in:
    # Huaxia Zhao and A. J. Reader, "Fast ray-tracing technique to calculate line integral paths in voxel arrays"
    # 2003 IEEE Nuclear Science Symposium. Conference Record (IEEE Cat. No.03CH37515), 2003, pp. 2808-2812 Vol.4,
    # doi: 10.1109/NSSMIC.2003.1352469.
    Ax0: float
    Ay0: float
    Jmin: float
    Jmax: float
    angle, distance = scan
    Lx = math.sin(math.radians(angle))
    Ly = math.cos(math.radians(angle))
    Rx = Lx * distance
    Ry = Ly * distance
    startX = round(xpos)
    startY = round(ypos)
    endX = round(xpos + Rx)
    endY = round(ypos + Ry)
    res = []

    def make_tuple(x_in, y_in) -> tuple:
        return x_in, y_in

    def swapped_tuple(x_in, y_in) -> tuple:
        return y_in, x_in

    pair = make_tuple

    if startX == endX:
        if Ry > 0:
            for y in range(startY + 1, endY + 1):
                res.append((startX, y))
            return res
        else:
            for y in range(startY - 1, endY - 1, -1):
                res.append((startX, y))
            return res
    if startY == endY:
        if Rx > 0:
            for x in range(startX + 1, endX + 1):
                res.append((startY, x))
            return res
        else:
            for x in range(startX - 1, endX - 1, -1):
                res.append((startY, x))
            return res

    # If the slope is greater than |1| then mirror the inputs and solve that way.
    swappedXY = True if ((angle + 45) // 90) % 2 == 0 else False
    if swappedXY:
        Rx, Ry = Ry, Rx
        startX, startY = startY, startX
        endX, endY = endY, endX
        pair = swapped_tuple
        Lx, Ly = Ly, Lx
        print("Swapped!")
    dy_dx = Ly / Lx
    Jmin = startY + 1 if Ry > 0 else startY
    Jmax = endY

    # First intersection point T(Tx, Ty) #
    # As boundaries are actually at 0.5/-0.5, 0.5 will be added to all values, so that (0, 0)(1, 1)
    # will define the pixel [0][0]
    xpos += 0.5
    ypos += 0.5
    xdir: int
    ydir: int

    # Find the X value distance between starting point and first point it crosses a whole Y value.
    if Rx > 0:
        Ax0 = math.fmod(xpos, 1)
        Ax0 = Ax0 - math.floor(Ax0)  # 0.4 -> 0.4, -0.4 -> 0.6
        xdir = 1
    else:
        Ax0 = math.fmod(xpos, 1)
        Ax0 = Ax0 - math.ceil(Ax0)  # 0.4 -> -0.6, -0.4 -> -0.4
        xdir = -1

    # Find distance between starting point and the point when it crosses the next whole Y value.
    if Ry > 0:
        Ay0 = math.fmod(ypos, 1)
        Ay0 = Ay0 - math.floor(Ay0)
        ydir = 1
    else:
        Ay0 = math.fmod(ypos, 1)
        Ay0 = Ay0 - math.ceil(Ay0)
        ydir = -1
    Ay0 = Ay0 / Ly
    Tx = xpos + Ay0 * Lx
    Ty = Jmin  # Ty would equal to Jmin

    Ax00, x_offset = math.modf(Ay0 * Lx + Ax0)  # Distance from farthest side compared to travel direction.
    print([startX, x_offset, xdir])
    for x_val in range(startX, startX + round(x_offset), xdir):
        # Add these starting pixels to the list
        res.append(pair(x_val, startY))
    x_val = startX + round(x_offset)
    y_val = startY + ydir
    res.append(pair(x_val, startY))  # Add extra pixel missed by loop

    if Rx > 0:  # Get new offset now that T(x, y) is found.
        Ax0 = Ax00 - math.floor(Ax00)
    else:
        Ax0 = Ax00 - math.ceil(Ax00)
    y_target = 1.0 - math.fabs(dy_dx)

    print("Before iterating!")
    print([y_val, x_val, Jmax, xdir, ydir, dy_dx, Ax0, Tx, Ty, Rx, Ry, res])
    while y_val != Jmax:
        y_acc = 0.0  # Accumulate up when counting blocks, then reset every loop.
        # Calculate A block
        x_offset = xdir - Ax0
        res.append(pair(x_val, y_val))
        y_acc += abs(x_offset * dy_dx)
        x_val += xdir

        # Add B blocks to the list.
        while y_acc <= y_target:
            y_acc += math.fabs(dy_dx)
            res.append(pair(x_val, y_val))
            x_val += xdir
        # Add a C block, get new offset then check if the end condition is met.
        res.append(pair(x_val, y_val))
        y_acc = (1 - y_acc) * ydir
        Ax0 = y_acc / dy_dx
        if Rx > 0:  # Get new offset.
            Ax0 = Ax0 - math.floor(Ax0)
        else:
            Ax0 = Ax0 - math.ceil(Ax0)
        y_val += ydir
    while x_val != endX:
        res.append(pair(x_val, y_val))
        x_val += xdir

    res.append(pair(endX, endY))
    return res


v1 = get_pixels_from_scan(0, 0, (73.2, 24.0))
v2 = get_pixels_from_scan(12.5, 23.1, (205.9, 15.3))
print(v1)
print(v2)
printing_matrix = []
for x in range(24):
    printing_matrix.append([])
    for y in range(24):
        printing_matrix[-1].append('O')
for tups in v1:
    printing_matrix[tups[0]][23 - tups[1]] = 'X'
printing_matrix[v1[-1][0]][23 - v1[-1][1]] = 'T'

for y in range(24):
    line_string = ''
    for x in range(24):
        line_string += printing_matrix[x][y]
        printing_matrix[x][y] = 'o'
    print(line_string)

print(printing_matrix)
for tups in v2:
    printing_matrix[tups[0]][23 - tups[1]] = 'x'
printing_matrix[v2[-1][0]][23 - v2[-1][1]] = 'T'
for y in range(24):
    line_string = ''
    for x in range(24):
        line_string += printing_matrix[x][y]
        printing_matrix[x][y] = 'o'
    print(line_string)



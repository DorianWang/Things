# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

MATRIX_SIZE = 4


def matrix_transpose_mult(m_in):
    for i in range(MATRIX_SIZE):
        for j in range(MATRIX_SIZE):
            value = 0
            for k in range(MATRIX_SIZE):
                value = value + m_in[i][k] * m_in[j][k]
            if i != j and value != 0:
                return False
            if i == j and value != 1:
                return False
    # print("Found a transposable one!")
    return True


def det_is_one(det_coeff, matrix):
    det_counter = 0
    for values in det_coeff:
        temp = 1
        for row in range(MATRIX_SIZE):
            temp *= matrix[row][int(values[row])-1]
        if values[4] == '+':
            det_counter = det_counter + temp
        elif values[4] == '-':
            det_counter = det_counter - temp
        else:
            raise ValueError
    if det_counter == 1:
        # print("Found one with determinant 1!")
        return True
    return False


def calc_rotations(det_coeff):
    tested_matrix = [[-1 for i in range(MATRIX_SIZE)] for j in range(MATRIX_SIZE)]
    for possible_matrix in range(43046721):  # 3^(MATRIX_SIZE*MATRIX_SIZE)
        if possible_matrix % 430467 == 0:
            print("\rCurrently finished " + str(possible_matrix // 430467) + " parts out of 100: ", end="")
        temp = possible_matrix
        current_multiplier = 1
        # Counts up and then sets the matrix values as though it was a trinary value.
        for i in range(MATRIX_SIZE-1, -1, -1):
            for j in range(MATRIX_SIZE-1, -1, -1):
                x = (temp // current_multiplier) % 3  # value of 0, 1, 2
                tested_matrix[i][j] = x - 1
                temp = temp - x * current_multiplier
                if temp == 0:
                    i = -1
                    break
                current_multiplier = current_multiplier * 3
        if det_is_one(det_coeff, tested_matrix) and matrix_transpose_mult(tested_matrix):
            print("Found one!")
            with open("output.txt", mode='at') as out_file:
                out_file.write("Found a rotation matrix!\n")
                for out_line in range(MATRIX_SIZE):
                    out_file.write("{:>3} {:>3} {:>3} {:>3}\n"
                                   .format(tested_matrix[out_line][0], tested_matrix[out_line][1],
                                           tested_matrix[out_line][2], tested_matrix[out_line][3]))


# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    determinant_coeff = []

    with open("detCalcs") as f:
        for line in f:
            line = line.strip(" \t\n\r")
            if line:
                determinant_coeff.append(line)
    calc_rotations(determinant_coeff)

    # matrix_transpose_mult([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]])

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

# This is a sample Python script.

import numpy as np
import math
import matplotlib.pyplot as mpl

input_arr = []
for t in range(1):  # 4 seconds
    for x in range(1000):  # 1000 samples per second
        input_arr.append(math.sin(t * 25 + x / 40) + math.cos(t * 123.4 + x * 0.1234) + 0.5)

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    fft_output = np.fft.fft(input_arr)
    freq = np.fft.fftfreq(len(fft_output), 0.001)
    fft_results = np.abs(np.fft.fft(input_arr)) / len(input_arr)
    mpl.plot(freq, fft_results)
    mpl.show()

# See PyCharm help at https://www.jetbrains.com/help/pycharm/

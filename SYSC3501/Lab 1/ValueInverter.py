import csv
import math

filename = "Scope_90_Phase.txt"


with open(filename, newline='') as f:
    with open(filename[0:-4] + "_reverse.txt", "w", newline='') as f2:
        reader = csv.reader(f, delimiter='\t')
        writer = csv.writer(f2, delimiter='\t')
        for row in reader:
            try:
                i1 = float(row[0])
            except ValueError:
                writer.writerow(row)
                continue
            i2 = float(row[1])
            i3 = float(row[2]) * -1.0
            writer.writerow([i1, i2, i3])





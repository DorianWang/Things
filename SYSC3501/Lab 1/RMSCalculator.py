import csv
import math

filename = "Scope_Square.txt"
rows = []


with open(filename, newline='') as f:
    reader = csv.reader(f, delimiter='\t')
    for row in reader:
        try:
            i1 = float(row[0])
        except ValueError:
            continue
        i2 = float(row[1])
        rows.append([i1, i2])
        #print(row)

#print(rows)
rawRMSSum = 0.0
rawAVGSum = 0.0
#print(len(rows))
for i in range(len(rows) - 1):
    rawRMSSum += (rows[i + 1][0] - rows[i][0]) * math.pow(((rows[i][1] + rows[i+1][1])/2), 2)

print(math.sqrt(rawRMSSum / (rows[-1][0] - rows[0][0])))  # RMS value

for i in range(len(rows) - 1):
    rawAVGSum += (rows[i + 1][0] - rows[i][0]) * ((rows[i][1] + rows[i + 1][1]) / 2)
print(rawAVGSum / (rows[-1][0] - rows[0][0]))  # Avg value


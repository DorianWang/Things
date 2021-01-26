import csv

filename = "I:\\Onedrive\\OneDrive - Carleton University\\FM_Spectra_3.csv"
outputname = "I:\\Onedrive\\OneDrive - Carleton University\\FM_Spectra_3.txt"

rows = []

with open(filename, newline='') as f1:
    with open(outputname, "w", newline='', ) as f2:
        reader = csv.reader(f1, delimiter='\t')
        writer = csv.writer(f2, delimiter=',', quotechar="\"")
        for row in reader:
            #row = row[0].split("\t")
            print(row)
            writer.writerow(row)

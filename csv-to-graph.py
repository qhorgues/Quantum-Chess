import csv
from matplotlib import pyplot as plt
import numpy as np

heuristique = []
move = []

with open('build/partie1.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=';')
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            heuristique.append(row[0])
            move.append(row[1])
            print(f'\t{row[0]} / {row[1]}.')
            line_count += 1
    print(f'Processed {line_count} lines.')

plt.plot(heuristique)
plt.axhline(y = 0, color='black')
plt.yticks(np.arange(-10, 10, 1))
plt.grid(axis='y', linestyle='-')
plt.show()
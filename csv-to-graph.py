import csv
from matplotlib import pyplot as plt
import numpy as np

heuristique = []
move = []

with open('partie1.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=';')
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            heuristique.append(float(row[0]))
            move.append(row[1])
            print(f'\t{row[0]} / {row[1]}.')
            line_count += 1
    print(f'Processed {line_count} lines.')

x = np.arange(1, len(move)+1, 1)
print(x)
print(move)
print(len(move))
plt.plot(heuristique)
plt.axhline(y = 0, color='black')
plt.grid(axis='y', linestyle='-')


def player(x):
    if x % 2 == 0:
        return 0
    else:
        return 3   

eval = np.array([move[i] == 'Split' for i in range(len(move))])
ymax1 = [player(i) for i in range(len(move))]
ymax2 = [-player(i+1) for i in range(len(move))]

for i in range(len(move)):
    if eval[i]:
        plt.plot([x[i], x[i]],[ymax1[i], ymax2[i]], color='red', alpha=0.5)

plt.show()
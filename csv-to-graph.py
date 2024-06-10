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
            line_count += 1
        else:
            heuristique.append(float(row[0]))
            move.append(row[1])
            print(f'\t{row[0]} / {row[1]}.')
            line_count += 1

x = np.arange(1, len(move)+1, 1)
print(x)
print(move)
print(len(move))
plt.plot(heuristique)
plt.title("évolution de l'heuristique au cours d'une partie")
plt.ylabel("Heuristique")
plt.xlabel("Nombre de coup joué (blanc ou noir)")
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


Board = np.array([[0, 0.4, 0.6, 0.7, 0.2, 0.5, 0.3, 0.1],
                  [0.7, 0.6, 0.9, 1.7, 1.2, 0.9, 0.2, 0.8],
                  [2.8, 2, 3.5, 3.9, 3.2, 3.6, 1.8, 1.3],
                  [2.4, 2, 3.9, 4.4, 4.5, 3.0, 1.6, 1.9],
                  [2.3, 1, 3.1, 4.8, 4.4, 3.9, 1.4, 1],
                  [2.2, 2, 1, 1.8, 3.1, 1.4, 1.8, 1.4],
                  [0.7, 1, 0.9, 2.5, 2.3, 0.6, 0.5, 0.2],
                  [0.9, 0.3, 0.7, 0.9, 0.4, 0.3, 0.2, 0.6]])


plt.imshow(Board, interpolation='none')
plt.title("Case joué le plus souvent en moyenne sur une succession de partie")
plt.show()



Board = np.array([[0, 0, 0, 0, 0, 0, 0, 0],
                  [0, 1, 1, 1, 1, 1, 1, 0],
                  [0, 1, 2, 2, 2, 2, 1, 0],
                  [0, 1, 2, 3, 3, 2, 1, 0],
                  [0, 1, 2, 3, 3, 2, 1, 0],
                  [0, 1, 2, 2, 2, 2, 1, 0],
                  [0, 1, 1, 1, 1, 1, 1, 0],
                  [0, 0, 0, 0, 0, 0, 0, 0]])
                


plt.imshow(Board, interpolation='none')
plt.title('Evaluation donné par eval pour toutes les cases')
plt.show()
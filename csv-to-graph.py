import csv
from matplotlib import pyplot as plt
import numpy as np
from os import listdir
from os.path import isfile, join
import os


N = 8
M = 8

def player(x, list_val):
    if x % 2 == 0:
        return 0
    else:
        return np.max(np.abs(list_val))   
    
def getLine(x):
    return int(x[0])

def getColumn(x):
    return int(x[1])

path = "save_chess/8x8/"
files = [f for f in listdir(path) if isfile(join(path, f))]

board = np.zeros([N, M])

for file in files:
    file_name, file_extension = os.path.splitext(file)
    if  file_extension == '.csv':
        heuristique = []
        move = []
        with open(path+file) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=';')
            line_count = 0
            for row in csv_reader:
                if line_count == 0:
                    line_count += 1
                else:
                    heuristique.append(float(row[0]))
                    move.append(row[1])
                    line_count += 1

                    if row[1] == 'Normal' or row[1] == 'Promote':
                        board[getLine(row[3]), getColumn(row[3])] += 1
                    elif row[1] == 'Split':
                        board[getLine(row[3]), getColumn(row[3])] += 1
                        board[getLine(row[4]), getColumn(row[4])] += 1
                    elif row[1] == 'Merge':
                        board[getLine(row[4]), getColumn(row[4])] += 1



        x = np.arange(1, len(move)+1, 1)
        plt.plot(heuristique, label='Heuristique')
        plt.title("évolution de l'heuristique au cours d'une partie")
        plt.ylabel("Heuristique")
        plt.xlabel("Nombre de coup joué (blanc ou noir)")
        plt.axhline(y = 0, color='black')
        plt.grid(axis='y', linestyle='-')

        eval = np.array([move[i] == 'Split' for i in range(len(move))])
        ymax1 = [player(i, heuristique) for i in range(len(move))]
        ymax2 = [-player(i+1, heuristique) for i in range(len(move))]

        for i in range(len(move)):
            if eval[i]:
                if i % 2 == 0:
                    color = 'red'
                else:
                    color = 'green'
                
                plt.plot([x[i], x[i]],[ymax1[i], ymax2[i]], color=color, alpha=0.5, label='Coup split')

        plt.show()


c = plt.imshow(board, interpolation='none')
plt.colorbar(c)
plt.title("Case joué le plus souvent en moyenne sur une succession de partie")
plt.show()

def eval(i, j):
    return 2*min(min(i, (N-i-1))/(N-2), min(j, (M-j-1))/(M-2))

eval_board = np.fromfunction(np.vectorize(eval), (N, M), dtype=float)

c = plt.imshow(eval_board, interpolation='none')
plt.colorbar(c)
plt.title('Evaluation donné par eval pour toutes les cases')
plt.show()
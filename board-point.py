import numpy as np
import matplotlib.pyplot as plt

Board = np.array([[0, 0.4, 0.6, 0.7, 0.2, 0.5, 0.3, 0.1],
                  [0.7, 0.6, 0.9, 1.7, 1.2, 0.9, 0.2, 0.8],
                  [2.8, 2, 3.5, 3.9, 3.2, 3.6, 1.8, 1.3],
                  [2.4, 2, 3.9, 4.4, 4.5, 3.0, 1.6, 1.9],
                  [2.3, 1, 3.1, 4.8, 4.4, 3.9, 1.4, 1],
                  [2.2, 2, 1, 1.8, 3.1, 1.4, 1.8, 1.4],
                  [0.7, 1, 0.9, 2.5, 2.3, 0.6, 0.5, 0.2],
                  [0.9, 0.3, 0.7, 0.9, 0.4, 0.3, 0.2, 0.6]])

Board /= np.max(Board) * 2.156
c = plt.imshow(Board, interpolation='none')
plt.colorbar(c)
plt.title("Cases le plus souvent jouées sur une succession de parties")
plt.show()
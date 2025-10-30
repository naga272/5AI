import matplotlib as mpt 
import matplotlib.pyplot as plt 
import numpy as np
import pandas as pd

# Quantita di un determinato bene che il consumatore e' disposto ad acquistare
# q = 80 - (10/3)*p
# dove p è il prezzo di 1 Gb
# Se vende 70 gb di spazio alla settimana?
# p = (80 - (10/3)) / q


def domanda(q: int) -> float:
    return (80 - q) / (10 / 3)


all_prezzo = []
all_quantita = [10, 20, 30, 40, 50, 60, 70]


for quantita in (all_quantita):
    all_prezzo.append(domanda(quantita) * quantita)


x = np.array(all_quantita)
y = np.array(all_prezzo)

plt.plot(x, y)

plt.xlabel("quantita")
plt.ylabel("Prezzo")
plt.title("Grafico della curva")
plt.savefig("./figura.png")
plt.show()


# Quantita di un determinato bene che il consumatore e' disposto ad acquistare
# q = 80 - (10/3)*p
# dove p è il prezzo di 1 Gb
# Se vende 70 gb di spazio alla settimana?
# p = (80 - (10/3)) / q

  
def domanda(q: int) -> float:
    return (80 - q) / (10 / 3)


all_quantita = [
    10, 20, 30, 40, 50, 60, 70
]

for quantita in all_quantita:
    print(domanda(quantita))

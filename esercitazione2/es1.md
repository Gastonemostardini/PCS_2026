# Esercizio 1
## Rappresentazione grafica delle posizioni in memoria
Dato il seguente output del programma,
```
ad
0x7fff7cbf0c50
0x7fff7cbf0c58
0x7fff7cbf0c60
0x7fff7cbf0c68

af
0x7fff7cbf0c30
0x7fff7cbf0c34
0x7fff7cbf0c38
0x7fff7cbf0c3c
0x7fff7cbf0c40
0x7fff7cbf0c44
0x7fff7cbf0c48
0x7fff7cbf0c4c

ai
0x7fff7cbf0c24
0x7fff7cbf0c28
0x7fff7cbf0c2c
```
dichiarando la posizione 0x7fff7cbf0c24 come base si possiamo estrarre il seguente schema per le posizioni in memoria:

| variabile | Byte | Posizione relative |
| :--- | :---: | ---: |
| ad[3] | □□□□□□□□ *8 | 68 |
| ad[2] | □□□□□□□□ *8 | 60 |
| ad[1] | □□□□□□□□ *8 | 52 |
| ad[0] | □□□□□□□□ *8 | 44 |
| af[7] | □□□□□□□□ *4 | 40 |
| af[...] | □□□□□□□□ *4 | ... |
| af[2] | □□□□□□□□ *4 | 20 |
| af[1] | □□□□□□□□ *4 | 16 |
| af[0] | □□□□□□□□ *4 | 12 |
| ai[2] | □□□□□□□□ *4 | 8 |
| ai[1] | □□□□□□□□ *4 | 4 |
| ai[0] | □□□□□□□□ *4 | 0 |

Complessivamente l'array ai occupa 12 Byte, af 32 Byte e ad 32.

## Motivazione 0
```
x:
1	0x7ffcc5d28200
y:
1.1 0x7ffcc5d281fc

x:
0	0x7ffcc5d28200
y:
1.1 0x7ffcc5d281fc
```
`x` è posizionato nella posizione di memoria subito successiva a `y` con l'operazione `(&y)[1]`
andiamo a toccare il suo valore poiche `&y` restituisce `0x7ffcc5d281fc` l'operatore `[1]` 
ad esso applicato ritorna `0x7ffcc5d281fc + 1 * 4` ovvero `0x7ffcc5d28200` che è esattamente 
l'indirizzo di `x` e `= 0;` lo sovrascrive.
In questo modo sovrascriviamo x con la riga `(&y)[1] = 0;`
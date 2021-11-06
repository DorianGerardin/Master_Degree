# TP - Programmation efficace d'algorithmes - GERARDIN Dorian

## Exercice 20 :

### 1. 
MT(n) = O(n²/B) avec M >= 2n²
En effet, dans les conditions optimales, nous stockons toutes les informations dans le cache.
Ici nous avons dans le cache, les 2 matrices qui sont chacunes de taille n², soit 2 x n². Avec O, les constantes sont négligeables.
Si chaque ligne contient B _doubles_ alors on se retrouve avec une complexité de  n²/B.
Sachant que M est la taille maximale du cache et que dans le meilleur des cas il contient les 2 matrices, nous avons la condition de M >= 2n².

### 2.
MT(n) = O(n²) si 2 < M/B <= n
Ici, dans le pire des cas, on ne peut pas prétendre garder B mots par lignes du caches. Sachant que nous stockons deux matrices de taille n², on obtient la complexité ci-dessus. On se retrouve dans le pire des cas lorsque M/B <= n car on ne peut pas stocker suffisamment de colonnes des matrices dans le cache et nous sommes obligés de le surcharger lorsque celui-ci est rempli.

### 3.


### 4.
_Voir dans le code_

Pour trouver une valeur de n dans le cas le plus favorable, il a fallu utiliser la commande 
```bash
getconf -a | grep CACHE
```
On obtient ceci : 

```bash
LEVEL1_ICACHE_SIZE                 32768
LEVEL1_ICACHE_ASSOC                8
LEVEL1_ICACHE_LINESIZE             64
LEVEL1_DCACHE_SIZE                 32768
LEVEL1_DCACHE_ASSOC                8
LEVEL1_DCACHE_LINESIZE             64
```

Ainsi, on apprend que l'ont peut stocker 4096 double dans le cache et 8 double par ligne.

On nous précise aussi que dans le cas optimal, M <= n(1+B)
on remplace par les bonnes valeurs : 
4096 >= n(1+8)
n <= 455

Cela signifie que si notre matrice carrée est de taille superieure à 455x455, on surcharge le cache et nous ne sommes plus dans un état optimal.

En appelant 50000 fois les fonctions de transposition de matrices ainsi que la fonction très simple de copie de tableau (qui est optimale pour du cache) avec un taille optimale (455), on obtient ceci :

```markdown
	Transpose matrix : 
nbr cycles : 13154129014 
nbr secondes : 7.30787
nbr millisecondes : 7307.87
CPI = 7.46836

	Copy matrix : 
nbr cycles : 6358567632
nbr secondes : 3.53255
nbr millisecondes : 3532.55
CPI = 3.61012
```


### 5.

_Voir dans le code_

On sait que M > 2b².
Or grâce à la question précédente, on sait que 
M = 32768
32768 > 2b²
16384 > b²
128 > b
On sait qu'un double est codé sur 8 octets donc il ne faut pas que b dépassent 16 doubles.

Voici un comparatif entre la méthode naïve et la méthode avec les blocks : (Appel 100 000 fois les méthodes, b = 16, taille matrice 128x128)

```markdown
	Transpose matrix : 
nbr cycles : 2592878627
nbr secondes : 1.44045
nbr millisecondes : 1440.45
CPI = 1.58257

	Transpose with blocks : 
nbr cycles : 1334069307
nbr secondes : 0.741133
nbr millisecondes : 741.133
CPI = 0.814251
```
On peut remarquer que la méthodes avec les blocks est environ 2 fois plus efficace que la méthode naïve.


### 6.

_Voir dans le code_

```markdown
 	 Transpose matrix naif: 
nbr cycles : 3201085364
nbr secondes : 1.77841
nbr millisecondes : 1778.41
CPI = 1.95379

	 Transpose with blocks : 
nbr cycles : 1774241654
nbr secondes : 0.985708
nbr millisecondes : 985.708
CPI = 1.08291
	 Transpose with Recursive : 
nbr cycles : 11535733079
nbr secondes : 6.40885
nbr millisecondes : 6408.85
CPI = 12.0114
```

Le nombre d'instructions de la méthode récursive est log(n)⁴ * 4. En effet, chaque appel de fonctions recursive fait log(n) instructions car on divise n par 2 et on s'arrête lorsque n == 1. De plus chaque appel de fonction rappelle 4 autres fonctions récursives d'où le log(n)⁴. On multiplie par 4 car il y a 4 appels recursifs.

On remarque tout de même avec les CPI que la méthode récursive n'est pas la plus optimale.


### 7.
	
_Voir dans le code_

```markdown
 	 Transpose matrix naif: 
nbr cycles : 2761897160
nbr secondes : 1.53441
nbr millisecondes : 1534.41
CPI = 1.68573

	 Transpose with blocks : 
nbr cycles : 1671557128
nbr secondes : 0.928655
nbr millisecondes : 928.655
CPI = 1.02024

	 Transpose with Recursive : 
nbr cycles : 11597315232
nbr secondes : 6.44303
nbr millisecondes : 6443.03
CPI = 12.0755

	 Transpose with Recursive min appels = 896: log2(n) * n (meilleure taille minimale)
nbr cycles : 2690393769
nbr secondes : 1.49468
nbr millisecondes : 1494.68
CPI = 2.80133
```


## Exercice 21 :

### 1.

_Voir dans le code_


### 2.

_Voir dans le code_


### 3.


### 4.


### 5.


### 6.

n = len(T)
log2(n/2)²

En effet, dans la fonction merge, on fait au maximum max(len(T1),len(T2))
Car lorsque le premier while se termine, (i1 < len(T1)) | (i2 < len(T2)) | (i1 = len(T1) & i2 = len(T2))
Ainsi dans la fonction merge on a max(len(T1),len(T2)) comparaisons.
Dans mergesort(T), les 2 tableaux font la même taille : n/2
Comme à chaque appel récursif on divise la taille par 2 et que l'on souhaite s'arrêter à 1, on fait log2(n/2) appels récursifs. 
On se retrouve donc avec un complexité asymptotique O(log2(n/2)²)


### 7.

MT(n) = 

```python
def merge(T1, T2):								
	n = len(T1) + len(T2)
	T = [0]*n  									⌈n/B⌉ + 1
	i1, i2, k = 0, 0, 0							⌈⌉
	while(i1 < len(T1) and i2 < len(T2)):
		if(T1[i1] < T2[i2]):
			T[k] = T1[i1]
			i1 += i1
		else :
			T[k] = T2[i2]
			i2 += 1
		k += 1
	while(i1 < len(T1)):
		T[k] = T1[i1]
		i1 += 1
		k += 1
	while(i2 < len(T2)):
		T[k] = T2[i2]
		i2 += 1
		k += 1
	print(T1, T2, T)
	return T
```
### 8.


### 9.


#### 10.







# Modélisation 3D - Animation TP2 - GERARDIN Dorian

## Exercice 1 :

### 1. 
J'ai crée un structure de données afin de représenter le futur squelette animé.
Pour cela, j'ai construit plusieurs classes : 
* Une articulation (un sommet)
* Un os (une arête)
* Un ensemble d'os
* Un squelette Articulé

Une articulation possède de nombreux attribut notamment une base, un angle, des coordonnées, 
une matrice de transformations etc..

_Voir fichier Dataset.js_

J'arrive bien a construire un squelette avec toutes ces propriétés.
Cependant, lors de cette construction les coordonnées des articulations ne sont pas dans les bonnes bases mais sont globales au canva. C'est pour cela que je n'ai pas réussi à animer le squelette. Je n'ai pas trop compris la bonne manière de construire la bonne structure de données afin d'attribuer les coordonnées des articulations dans la bonne base.



# HMIN322 : idc-3D

Projet *template* pour le TP sur l'insertion de données cachées dans les objets 3D.
Ce document contient la liste des activités à réaliser durant le TP.

## VERIFIER LA FIN DU DOCUMENT POUR LES CONDITIONS DE RENDU DE TP

## Pré-requis

- git
- cmake
- g++/gcc
- make
- libblas-dev liblapack-dev (blas api)
- freeglut3-dev (OpenGL)
- libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev (RandR, Xinerama, XCursor, Xi)
(sudo apt-get install <libname>)

## Préparation

- Cloner le dépôt
```sh
git clone https://github.com/TsubameDono/hmin322-idc-3D.git
cd hmin322-idc-3D
git submodule update --init --recursive
```
- Compiler le projet de base. Construit automatiquement la bibliothèque **libigl** et télécharger toutes les bibliothèques externes (eigen, etc.)
```sh
$ mkdir build # create build directory
$ cd build # enter build directory
$build/ cmake .. # build libigl project (automatically load external libs)
#$ build/ cmake .. -G "Visual Studio 15 2017 Win64" # for window
$build/ make # build libraries
OR
$build/ cmake --build .
```

## Explication

**libigl** : librairie contenant des méthodes de traitement des objets 3D (IO, lissage, remaillage, filtrage, métrique)
 - igl::readXXX(.) (OFF, STL, etc.)
 - igl::writeXXX(.)
 - igl::hausdorff(.)
 - igl::is_border_vertex(.)
**libigl/opengl** : sous-librairie permettant de prototyper un outil de rendu avec igl
```cpp
#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>

Eigen::MatrixXd V;
Eigen::MatrixXi F;

int main(int argc, char *argv[])
{
  // Load a mesh in OFF format
  igl::readOFF(TUTORIAL_SHARED_PATH "/bunny.off", V, F);

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.launch();
}
```
**eigen** : librairie de structures mathématiques matricielles pour la représentation
 - Eigen::MatrixXf ou Eigen::MatrixXd pour les coordonnées, Eigen::MatrixXi pour les indices
```cpp
Eigen::MatrixXd V;
Eigen::MatrixXi F;
```

## Travaux

### 1. Importer un maillage (uniquement pour ceux n'ayant pas de projets 3D)
1. Clôner le dépôt
2. Charger un maillage 3D à l'aide de la bibliothèque **libigl** en utilisant la fonction *read_triangle(.)*
3. Sauvegarder une copie du maillage 3D à l'aide de la bibliothèque **libigl** en utilisant la fonction *write_triangle(.)*

### 2. Insertion de données cachées

Nous allons implémenter la méthode de Cho *et al.* (2007) [papier](https://www.researchgate.net/profile/Remy_Prost/publication/3319967_An_Oblivious_Watermarking_for_3-D_Polygonal_Meshes_Using_Distribution_of_Vertex_Norms/links/02e7e52849d95aa0ea000000/An-Oblivious-Watermarking-for-3-D-Polygonal-Meshes-Using-Distribution-of-Vertex-Norms.pdf)

1. Ecrire une fonction calculant le barycentre d'un maillage
2. Ecrire une fonction convertissant les coordonnées cartésiennes en coordonnées sphériques
3. Ecrire une fonction convertissant les coordonnées sphériques en coordonnées cartésiennes
4. Ecrire une fonction trouvant la valeur minimale et la valeur maximale de la coordonnée radiale des sommets
5. Ecrire une fonction normalisant un ensemble de valeurs [x_0, x_1, ..., x_n] entre 0.0 et 1.0 (en fonction du minimum et du maximum)
6. Ecrire une fonction dénormalisant un ensemble de valeurs comprises entre 0.0 et 1.0 (en fonction d'un minimum et d'un maximum)
7. Ecrire une fonction qui trie un ensemble de valeurs en histogramme de k *bins*
7. Ecrire la fonction de **tatouage** prenant en entré un maillage, un message, le nombre de bins, *alpha* et la force d'insertion
8. Ecrire la fonction d'extraction du message à partir d'un maillage, le nombre de bins et *alpha*

### 3. Présenter les résultats

Présenter dans un rapport (sous forme de tableau, courbes) les résultats en terme de distorsions selon des métriques pour un même maillage.
Métriques obligatoires : BER, RMSE, PSNR
1. En faisant varier la force d'insertion
2. En faisant varier la capacité

### 4. Attaques

Créer des attaques contre la méthode.

1. Attaque de la robustesse : écrire une fonction enlevant le message du maillage marqué

2. Attaque de la sécurité : écrire une fonction estimant le contenu du message du maillage marqué

### 5. Bonus

Proposer des améliorations soit :
- Sécurité
- Capacité
- Robustesse

# RENDU
## Rapport à rendre avant le 30 septembre 2019 à l'adresse suivante : sebastien.beugnon[at]lirmm.fr
## Objet du mail : [HMIN322] IDC 3D - <NOM.ETUDIANT>

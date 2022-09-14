// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <climits>  
#include <algorithm> 
#include <vector>
#include <cmath>
#include <cfloat>
#include "image_ppm.h"
#include "file.h"

using namespace std;

void histogram(OCTET *data, int size, int *histo) {
  for (int i = 0; i < 256; ++i)
  {
    histo[i] = 0;
  }

  for (int i = 0; i < size; ++i)
  {
    histo[data[i]]++;
  }
} 

float entropy(int *histo, int size) {
  float sum = 0;
  for (int i = 0; i < 256; ++i)
  {
    float ddp = (float) histo[i] / (float) size;
    if(ddp != 0) sum -= ddp*log2(ddp);
  }
  return sum;
}

struct Image {
  OCTET *data;
  OCTET *out;
  char* filename;
  int size, nW, nH;

  void permutation(unsigned int key) {
    int posMap[size];
    srand(key);
    for (int i = 0; i < size; ++i)
    {
      posMap[i] = -1;
    }

    for (int i = 0; i < size; ++i)
    {
      int pos = rand() % size;
      while(posMap[pos] != -1) {
        if(pos == size-1) pos = 0;
        else pos++;
      }
      out[pos] = data[i];
      posMap[pos] = i;
    }
  }

  void decryptPermutation(unsigned int key) {
    int posMap[size];
    srand(key);
    for (int i = 0; i < size; ++i)
    {
      posMap[i] = -1;
    }

    for (int i = 0; i < size; ++i)
    {
      int pos = rand() % size;
      while(posMap[pos] != -1) {
        if(pos == size-1) pos = 0;
        else pos++;
      }
      out[i] = data[pos];
      posMap[pos] = i;
    }
  }

  void substitution(unsigned int key) {
    srand(key);
    int k;
    k = rand() % 256;
    out[0] = k + data[0];
    for (int i = 1; i < size; ++i)
    {
      k = rand() % 256;
      out[i] = (data[i] + out[i-1] + k) % 256;
    }
  }

  void decryptSubstitution(unsigned int key) {
    srand(key);
    int k;
    k = rand() % 256;
    out[0] = data[0] - k;
    for (int i = 1; i < size; ++i)
    {
      k = rand() % 256;
      out[i] = (data[i] - data[i-1] - k) % 256;
    }
  }

  void bruteForceSubstitution() {
    float minEntropy = FLT_MAX;
    int bestImg = 1;
    for (int i = 1; i < 256; ++i)
    {
      decryptSubstitution(i);
      int histo[256];
      histogram(out, size, histo);
      float e = entropy(histo, size);
      if(e < minEntropy) {
        minEntropy = e;
        bestImg = i;
      }
    }
    printf("La clé est : %i\n", bestImg);
    decryptSubstitution(bestImg);
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], mode[100];
  int nH, nW, nTaille; 
  unsigned int key;

  if (argc != 5) 
     {
       printf("Usage: ImageIn ImageOut key mode\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
  sscanf (filename1,"%s", cNomImgLue);
  sscanf (filename2,"%s", cNomImgEcrite);
  sscanf (argv[4],"%s", mode);
  sscanf (argv[3],"%u", &key);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = cNomImgLue;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
  };

  allocation_tableau(img->data, OCTET, img->size);
  allocation_tableau(img->out, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, img->nH * img->nW);

  if(strcmp(mode, "perm") == 0) img->permutation(key);
  else if(strcmp(mode, "sub") == 0) img->substitution(key);
  else if(strcmp(mode, "decPerm") == 0) img->decryptPermutation(key);
  else if(strcmp(mode, "decSub") == 0) img->decryptSubstitution(key);
  else if(strcmp(mode, "bruteForce") == 0) img->bruteForceSubstitution();
  else {
    printf("Incorrect mode\n"); 
    exit (1) ;
  }

  ecrire_image_pgm(cNomImgEcrite, img->out, img->nH, img->nW);
  free(img->data);
  free(img->out);
  return 1;
}

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
#include "AES.h"

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

  void extract(unsigned int key, unsigned int lsb) {
    srand(key);
    unsigned int littleImgSize = 64*64;
    for (int i = 0; i < littleImgSize; ++i)
    {
      for (int b = 0; b < 8; ++b)
      {
        int rpos = rand()%size;
        unsigned char bit = data[rpos] & (1 << lsb);
        if(bit) out[i] = out[i] | (1 << b);
        else out[i] = out[i] & ~ (1 << b);
      }
    }
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], mode[100];
  int nH, nW, nTaille; 
  unsigned int key, lsb;

  if (argc != 5) 
  {
     printf("Usage: ImageIn ImageOut key lsb\n"); 
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
  sscanf (argv[3],"%u", &key);
  sscanf (argv[4],"%u", &lsb);

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
  allocation_tableau(img->out, OCTET, 64*64);
  lire_image_pgm(img->filename, img->data, img->size );

  img->extract(key, lsb);

  ecrire_image_pgm(cNomImgEcrite, img->out, 64, 64);

  free(img->data);
  free(img->out);
  free(filename1);
  free(filename2);
  return 1;
}

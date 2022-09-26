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

  void insert(unsigned int key, unsigned int lsb, Image *toHide) {
    for (int i = 0; i < size; ++i)
    {
      out[i] = data[i];
    }

    srand(key);
    for (int i = 0; i < toHide->size; ++i)
    {
      for (int b = 0; b < 8; ++b)
      {
        unsigned char bit = toHide->data[i] & (1 << b);
        unsigned int rpos = rand()%size;

        if(!(data[rpos] & (1 << lsb))) {
          if(bit) out[rpos] = out[rpos] | (1 << lsb);
        }
        else {
          if(!bit) out[rpos] = out[rpos] & ~ (1 << lsb);
        }
      }
    }
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], cNomImgHide[250];
  int nHBig, nWBig, nTailleBig; 
  int nHHide, nWHide, nTailleHide; 
  unsigned int key, lsb;

  if (argc != 6) 
  {
     printf("Usage: ImageIn ImageHide ImageOut key lsb\n"); 
     exit (1) ;
  }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  string s3 = string(folder) + string(argv[3]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
  char* filename3 = stringToCharArray(s3);
   
  sscanf (filename1,"%s", cNomImgLue);
  sscanf (filename2,"%s", cNomImgHide);
  sscanf (filename3,"%s", cNomImgEcrite);
  sscanf (argv[4],"%u", &key);
  sscanf (argv[5],"%u", &lsb);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nHBig, &nWBig);
  lire_nb_lignes_colonnes_image_pgm(cNomImgHide, &nHHide, &nWHide);
  nTailleBig = nHBig * nWBig;
  nTailleHide = nHHide * nWHide;

  Image *bigImg = new Image();
  {
    bigImg->filename = cNomImgLue;
    bigImg->size = nTailleBig;
    bigImg->nH = nHBig;
    bigImg->nW = nWBig;
  };

  Image *imgHide = new Image();
  {
    imgHide->filename = cNomImgHide;
    imgHide->size = nTailleHide;
    imgHide->nH = nHHide;
    imgHide->nW = nWHide;
  };

  allocation_tableau(bigImg->data, OCTET, bigImg->size);
  allocation_tableau(bigImg->out, OCTET, bigImg->size);
  allocation_tableau(imgHide->data, OCTET, imgHide->size);
  lire_image_pgm(bigImg->filename, bigImg->data, bigImg->size );
  lire_image_pgm(imgHide->filename, imgHide->data, imgHide->size );

  bigImg->insert(key, lsb, imgHide);

  ecrire_image_pgm(cNomImgEcrite, bigImg->out, bigImg->nH, bigImg->nW);

  free(bigImg->data);
  free(bigImg->out);
  free(imgHide->data);
  free(filename1);
  free(filename2);
  free(filename3);
  return 1;
}

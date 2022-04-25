// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include "image_ppm.h"
#include "file.h"

using namespace std;

struct Image {
  OCTET *cannyEdges;
  OCTET *data;
  OCTET *out;
  char* filename;
  int size, nW, nH;

  void updateData() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        data[i*nW+j] = out[i*nW+j];
      }
    }
  }

  void impaiting() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        if(data[i*nW+j] == 0 && cannyEdges[i*nW+j] == 255) {
          cout << "je suis une arete de la target" << endl;
          if(i*nW+j-1 >= 0) out[i*nW+j] = data[i*nW+j-1];
        } else {
          out[i*nW+j] = data[i*nW+j];
        }
      }
    }
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cannyEdges[250], cNomImgEcrite[250];
  int nH, nW, nTaille; 
  
  if (argc != 4) 
     {
       printf("Usage: CannyEdges ImageIn ImageOut\n"); 
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
   
  sscanf (filename1,"%s",cannyEdges) ;
  sscanf (filename2,"%s",cNomImgLue);
  sscanf (filename3,"%s",cNomImgEcrite);

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
  allocation_tableau(img->cannyEdges, OCTET, img->size);
  allocation_tableau(img->out, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, nH * nW);
  lire_image_pgm(filename1, img->cannyEdges, nH * nW);

  img->impaiting();

  ecrire_image_pgm(cNomImgEcrite, img->out, img->nH, img->nW);
  free(img->data);
  free(img->cannyEdges);
  free(img->out);
  return 1;
}

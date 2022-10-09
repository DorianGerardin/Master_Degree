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
#include <array>
#include <cmath>
#include <cfloat>
#include "image_ppm.h"
#include "file.h"

using namespace std;

struct Image {
  OCTET *data;
  OCTET *out;
  char* filename;
  char* path;
  char* pathOut;
  int size, nW, nH;
};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], type[100];
  int nH, nW, nTaille; 

  if (argc != 3) 
  {
     printf("Usage: ImageOut type\n"); 
     exit (1) ;
  }

  const char *datFile;
  const char *extension = ".pgm";

  sscanf (argv[2],"%s", type);
  if(strcmp(type, "bear") == 0) {
    datFile = "bearVectors2.dat";
  } 
  else if(strcmp(type, "boar") == 0) {
    datFile = "boarVectors2.dat";
  }

  string s1 = string(argv[1]) + string(extension);
  char* path1 = stringToCharArray(s1);
   
  sscanf (path1,"%s", cNomImgEcrite);

  Image *img = new Image();
  {
    img->path = cNomImgEcrite;
    img->pathOut = cNomImgEcrite;
    img->size = 102400;
    img->nH = 1;
    img->nW = 102400;
  };

  allocation_tableau(img->data, OCTET, img->size);
 
  fstream my_file;
  my_file.open(datFile, ios::in);
  if (!my_file) {
    cout << "No such file";
  }
  else {
    int cpt = 0;
    string line;
    while (std::getline(my_file, line)){
      char* lineChar = stringToCharArray(line);
      int value = atoi(lineChar);
      img->data[cpt] = value;
      free(lineChar);
    }
  }
  my_file.close();

  ecrire_image_pgm(img->pathOut, img->data, img->nH, img->nW);

  free(img->data);
  free(path1);
  return 1;
}

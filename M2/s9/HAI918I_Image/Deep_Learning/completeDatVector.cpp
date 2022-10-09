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
#include <sstream>
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
     printf("Usage: ImageIn type\n"); 
     exit (1) ;
  }

  const char *datFile;
  const char *folder;
  const char *extension = ".pgm";

  sscanf (argv[2],"%s", type);
  if(strcmp(type, "bear") == 0) {
    datFile = "bearVectors2.dat";
    folder = "./bearVectorTrain/";
  } 
  else if(strcmp(type, "boar") == 0) {
    datFile = "boarVectors2.dat";
    folder = "./boarVectorTrain/";
  }
  
  string s1 = string(folder) + string(argv[1]) + string(extension);
  char* path1 = stringToCharArray(s1);
   
  sscanf (path1,"%s", cNomImgLue);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = argv[1];
    img->path = cNomImgLue;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
  };

  allocation_tableau(img->data, OCTET, img->size);
  lire_image_pgm(img->path, img->data, img->size);

  fstream my_file;
  fstream tempFile; //Temporary file
  my_file.open(datFile, ios::in);
  tempFile.open("temp.dat", ios::out);
  if (!my_file) {
    cout << "No such file";
  }
  else {
    int cpt = 0;
    string line;
    while (std::getline(my_file, line)){
      char* lineChar = stringToCharArray(line);
      float value = atof(lineChar);
      float newValue = ((float)img->data[cpt] / 47.) + value;
      tempFile << newValue << endl;
      cpt++;
      free(lineChar);
    }
  }
  remove(datFile);
  rename("temp.dat", datFile);
  my_file.close();
  tempFile.close();


  free(img->data);
  free(path1);
  return 1;
}

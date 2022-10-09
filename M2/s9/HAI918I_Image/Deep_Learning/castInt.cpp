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

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], type[100];
  int nH, nW, nTaille; 

  if (argc != 2) 
  {
     printf("Usage: type\n"); 
     exit (1) ;
  }

  const char *datFile;
  const char *folder;
  const char *extension = ".pgm";

  sscanf (argv[1],"%s", type);
  if(strcmp(type, "bear") == 0) {
    datFile = "bearVectors2.dat";
  } 
  else if(strcmp(type, "boar") == 0) {
    datFile = "boarVectors2.dat";
  }

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
      int newValue = (int)round(value);
      tempFile << newValue << endl;
      cpt++;
      free(lineChar);
    }
  }
  remove(datFile);
  rename("temp.dat", datFile);
  my_file.close();
  tempFile.close();

  return 1;
}

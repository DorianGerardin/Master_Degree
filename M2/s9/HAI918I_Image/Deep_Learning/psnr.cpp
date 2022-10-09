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

int main(int argc, char* argv[])
{
  char cNomImgLue1[250], cNomImgLue2[250], type[100];
  int nH, nW, nTaille, direction;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn1 ImageIn2 type\n"); 
       exit (1) ;
     }

  const char *folder;
  const char *extension = ".pgm";

  sscanf (argv[3],"%s", type);
  if(strcmp(type, "bear") == 0) {
    folder = "./bearVectorTest/";
  } 
  else if(strcmp(type, "boar") == 0) {
    folder = "./boarVectorTest/";
  }

  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* path1 = stringToCharArray(s1);
  char* path2 = stringToCharArray(s2);
   
  sscanf (path1,"%s", cNomImgLue1);
  sscanf (path2,"%s", cNomImgLue2);

  OCTET *ImgIn1,*ImgIn2;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH, &nW);
   nTaille = nH * nW;

  int nTaille3 = nTaille * 3;
  allocation_tableau(ImgIn1, OCTET, nTaille);
  lire_image_pgm(cNomImgLue1, ImgIn1, nH * nW);
  allocation_tableau(ImgIn2, OCTET, nTaille);
  lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);
  float eqmR=0, eqmG=0, eqmB=0;
   
   
  for (int i=0; i < nTaille; i++)
  {
    eqmR += (float)(ImgIn1[i] - ImgIn2[i])*(ImgIn1[i] - ImgIn2[i]);  
  } 
  eqmR /= (nH*nW);

  float psnrR = 10 * log10 ((255*255)/eqmR);
  printf("PSNR : %f \n", psnrR);
  
  free(ImgIn1);
  free(ImgIn2);
  return 1;
}
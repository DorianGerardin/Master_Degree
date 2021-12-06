// blur.cpp : Floute une image en niveau de gris

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "image_ppm.h"
using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  int som;
  if (argc != 3) 
     {
       printf("Usage: ImageIn ImageOut \n"); 
       exit (1) ;
     }

  const char *folder = "./img/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  int filenameSize1 = s1.length();
  int filenameSize2 = s2.length();
  char filename1[filenameSize1+1];
  char filename2[filenameSize2+1];
  strcpy(filename1, s1.c_str());
  strcpy(filename2, s2.c_str());
  sscanf (filename1,"%s",cNomImgLue) ;
  sscanf (filename2,"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
	
   for (int i=0; i < nTaille; i++)
    ImgOut[i]= ImgIn[i];

 for (int i=1; i < nH-1; i++)
   for (int j=1; j < nW-1; j++)
     {
		som=
		 ImgIn[(i-1)*nW+j-1]+ImgIn[(i-1)*nW+j]+ImgIn[(i-1)*nW+j+1]
		+ImgIn[i*nW+j-1]+ImgIn[i*nW+j]+ImgIn[i*nW+j+1]
		+ImgIn[(i+1)*nW+j-1]+ImgIn[(i+1)*nW+j]+ImgIn[(i+1)*nW+j+1];
		
       ImgOut[i*nW+j]=som/9;
     }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);free(ImgOut);
   return 1;
}
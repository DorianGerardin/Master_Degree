// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgLue2[250], cNomImgEcrite[250];
   int nH, nW, nTaille;
  
  if (argc != 4) 
     {
       printf("Usage: ImageSeuil ImageDilate ImageOut \n"); 
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
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgLue2) ;
   sscanf (filename3,"%s",cNomImgEcrite);

   OCTET *ImgInSeuil, *ImgInDilate, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgInSeuil, OCTET, nTaille);
   allocation_tableau(ImgInDilate, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgInSeuil, nH * nW);
   lire_image_pgm(cNomImgLue2, ImgInDilate, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
      if(ImgInSeuil[i*nW+j] == 0 && ImgInDilate[i*nW+j] == 0) {
        ImgOut[i*nW+j] = 0;
      } else if(ImgInSeuil[i*nW+j] == 255 && ImgInDilate[i*nW+j] == 255) {
        ImgOut[i*nW+j] = 0;
      } else if(ImgInSeuil[i*nW+j] == 0 && ImgInDilate[i*nW+j] == 255) {
        ImgOut[i*nW+j] = 255;
      }
     }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgInSeuil);
   free(ImgInDilate);
   return 1;
}

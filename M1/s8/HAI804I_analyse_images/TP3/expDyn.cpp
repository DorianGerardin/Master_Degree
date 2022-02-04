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
  int histo[256] = {0};
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   for (int i=0; i < nH; i++) {
     for (int j=0; j < nW; j++)
      {
        histo[ImgIn[i*nW+j]]++;
      }
   }

   bool a0Trouve = false;
   int a0 = 0;
   int a1 = 0;

   for (int i = 0; i < 256; ++i)
   {
     if(histo[i] > 0) {
      a1 = i;
      if(!a0Trouve) {
        a0Trouve = true;
        a0 = i;
      }
     }
   }

   float alpha = -255 * a0 / (float)(a1-a0);
   float beta = 255 / (float)(a1-a0);

   for (int i=0; i < nH; i++) {
     for (int j=0; j < nW; j++)
      {
        ImgOut[i*nW+j] = alpha + beta*ImgIn[i*nW+j];
      }
   }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

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
  float ddp[256] = {0};
  int histo[256] = {0};
  float F[256] = {0};
  float T[256] = {0};
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm imageOut\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgEcrite) ;

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

  for (int i=0; i < nTaille; i++) {
      histo[ImgIn[i]]++;
   }

   for (int i=0; i < 256; i++) {
      ddp[i] = (float)histo[i] / (float)nTaille;
   }

   /*for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << ddp[i] << endl;
   }*/

   for (int i = 0; i < 256; ++i)
   {
     for (int j = 0; j < i; ++j)
     {
       F[i] += ddp[j];
     }
   }

   /*for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << F[i] << endl;
   }*/

   for (int i = 0; i < 256; ++i)
   {
     T[i] = 255 * F[i];
   }

   for (int i = 0; i < nTaille; ++i)
   {
     ImgOut[i] = T[(int)floor(ImgIn[i])];
   }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

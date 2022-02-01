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
  char cNomImgLue[250];
  int histo[256] = {0};
   int nH, nW, nTaille;
  
  if (argc != 2) 
     {
       printf("Usage: ImageIn.pgm \n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  char* filename1 = stringToCharArray(s1);
   
   sscanf (filename1,"%s",cNomImgLue) ;

   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
	
  
  for (int i=0; i < nH; i++) {
   for (int j=0; j < nW; j++)
     {
        histo[ImgIn[i*nW+j]]++;
     }
   }

   for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << histo[i] << endl;
   }

   free(ImgIn);
   return 1;
}

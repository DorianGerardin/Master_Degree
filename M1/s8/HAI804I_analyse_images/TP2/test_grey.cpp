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
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, S;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm Seuil \n"); 
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
   sscanf (argv[3],"%d",&S);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
	
   //   for (int i=0; i < nTaille; i++)
   // {
   //  if ( ImgIn[i] < S) ImgOut[i]=0; else ImgOut[i]=255;
   //  }


 for (int i=0; i < nH; i++)
   for (int j=0; j < nW; j++)
     {
       if ( ImgIn[i*nW+j] < S) ImgOut[i*nW+j]=0; else ImgOut[i*nW+j]=255;
     }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

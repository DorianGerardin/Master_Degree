#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <cmath>
#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn ImageOut\n"); 
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
	
  ImgOut[0] = 0;
  for (int i = 1; i < nW; ++i)
  {
    int diff = ( ImgIn[i] - ImgIn[i-1] ) + 128;
    if(diff < 0) diff = 0;
    if(diff > 255) diff = 255;
    ImgOut[i] = diff;
  }

  for (int i = 1; i < nH; ++i)
  {
    int diff = ( ImgIn[i*nW] - ImgIn[(i-1)*nW] ) + 128;
    if(diff < 0) diff = 0;
    if(diff > 255) diff = 255;
    ImgOut[i*nW] = diff;
  }

  int diff;
  for (int i=1; i < nH; i++) {
   for (int j=1; j < nW; j++)
     {
      int AB = abs(ImgIn[i*nW+(j-1)] - ImgIn[(i-1)*nW+(j-1)]);
      int BC = abs(ImgIn[(i-1)*nW+j] - ImgIn[(i-1)*nW+(j-1)]);
     	if(AB < BC) {
        diff = ImgIn[(i-1)*nW+j] + 128;
      } else {
        diff = ImgIn[i*nW+(j-1)] + 128;
      }
        if(diff < 0) diff = 0;
  	    if(diff > 255) diff = 255;
  	    ImgOut[i*nW+j] = diff;
     }
     
   }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

#include <stdio.h>
#include "image_ppm.h"
#include <math.h>
//#include <cmath>
//#include <iostream>
#include <stdlib.h>


int main(int argc, char* argv[])
{
  char cNomImgLue1[250], cNomImgLue2[250];
  int nH, nW, nTaille, direction;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn1.ppm ImageIn2.ppm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue1) ;
   sscanf (argv[2],"%s",cNomImgLue2) ;

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
  printf("\npsnr  : %f \n", psnrR);
  
  free(ImgIn1);
  free(ImgIn2);
  return 1;
}
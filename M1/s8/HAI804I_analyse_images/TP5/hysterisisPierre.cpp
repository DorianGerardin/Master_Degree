// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include <math.h>
#include "image_ppm.h"
#include <iostream>

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250] = "hysterisis.pgm";
  int nH, nW, nTaille, SB, SH;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm SB SH\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%d",&SB) ;
   sscanf (argv[3],"%d",&SH) ;

   OCTET *ImgIn, *ImgOut, *ImgHyst;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);
   allocation_tableau(ImgHyst, OCTET, nTaille);
	
for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
      {
      if(i == 0 || j == 0 || i == nH-1 || j == nW-1) ImgHyst[i*nW+j] = 0;
      else if(ImgIn[i*nW+j]<SH) ImgHyst[i*nW+j] = ImgIn[i*nW+j];
      else ImgHyst[i*nW+j] = 255;
      }

  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j++)
      {
          if(SB < ImgHyst[i*nW+j]) {
              // Si l'un des contours est blanc
              if(i-1>=0 && j-1>=0 && ImgHyst[(i-1)*nW+(j-1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i-1,j-1)
              else if(j-1>=0 && ImgHyst[i*nW+(j-1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i,j-1)
              else if(i+1<nH && j-1>=0 && ImgHyst[(i+1)*nW+(j-1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i+1,j-1)
              else if(i-1>=0 && ImgHyst[(i-1)*nW+j]==255) 
                    ImgOut[i*nW+j] = 255; // p(i-1,j)
              else if(i+1<nH && ImgHyst[(i+1)*nW+j]==255) 
                    ImgOut[i*nW+j] = 255; // p(i+1,j)
              else if(i-1>=0 && j+1<nW && ImgHyst[(i-1)*nW+(j+1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i-1,j+1)
              else if(j+1<nW && ImgHyst[i*nW+(j+1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i,j+1)
              else if(i+1<nH && j+1<nW && ImgHyst[(i+1)*nW+(j+1)]==255) 
                    ImgOut[i*nW+j] = 255; // p(i+1,j+1)
              // Sinon pixel noir
              else ImgOut[i*nW+j] = 0;
          }
          else ImgOut[i*nW+j] = 0;
      }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

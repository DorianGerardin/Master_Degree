#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

void transformeOndelette(OCTET *ImgIn, char* filenameIn, int nH, int nW, 
                         OCTET *ImgOut, char* filenameOut, int N) {
  lire_nb_lignes_colonnes_image_pgm(filenameIn, &nH, &nW);
  int nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  lire_image_pgm(filenameIn, ImgIn, nH * nW);
  int A, B, C, D;

  if (N = 0) {
    //ecrire_image_pgm(filenameOut, ImgOut,  nH, nW);
  } else {
    for (int i=0; i < nH; i+=2) {
     for (int j=0; j < nW; j+=2)
      {
        A = ImgIn[i*nW+j];
        B = ImgIn[i*nW+j+1];
        C = ImgIn[(i+1)*nW+j];
        D = ImgIn[(i+1)*nW+j+1];

        std::cout << "i : " << i << std::endl;
        std::cout << "j : " << j << std::endl;

        std::cout << "A : " << A << std::endl;
        std::cout << "B : " << B << std::endl;
        std::cout << "C : " << C << std::endl;
        std::cout << "D : " << D << std::endl;

        int BF = (A+B+C+D)/4;
        int MFv = (A+B-C-D)/2;
        int MFh = (A-B+C-D)/2;
        int HF = A-B-C+D;

        //en haut a gauche
        if(i >= 0 && i <= 255 && j >= 0 && j <= 255) {
          ImgOut[(i/2)*nW+(j/2)] = BF;
          ImgOut[(i/2 + 256)*nW+(j/2 + 256)] = HF;
          ImgOut[(i/2 + 256)*nW+(j/2)] = MFh;
          ImgOut[(i/2)*nW+(j/2  + 256)] = MFv;
        }
        //en haut a droite
        if(i >= 0 && i <= 255 && j >= 256 && j <= 511) {
          ImgOut[(i/2)*nW+(j/2)] = MFv;
          ImgOut[(i/2 + 256)*nW+(j/2)] = HF;
          ImgOut[(i/2 + 256)*nW+(j/2 - 256)] = MFh;
          ImgOut[(i/2)*nW+(j/2 - 256)] = BF;
        }
        //en bas a gauche
        if(i >= 256 && i <= 511 && j >= 0 && j <= 255) {
          ImgOut[(i/2)*nW+(j/2)] = MFh;
          ImgOut[(i/2)*nW+(j/2 + 256)] = HF;
          ImgOut[(i/2 - 256)*nW+(j/2 + 256)] = MFh;
          ImgOut[(i/2 - 256)*nW+(j/2)] = BF;
        }
        //en bas a droite
        if(i >= 256 && i <= 511 && j >= 256 && j <= 511) {
          ImgOut[(i/2)*nW+(j/2)] = HF;
          ImgOut[(i/2)*nW+(j/2 - 256)] = MFh;
          ImgOut[(i/2 +-256)*nW+(j/2)] = MFv;
          ImgOut[(i/2 - 256)*nW+(j/2 - 256)] = BF;
        }
      }
   }
  }
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, N;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn\n"); 
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
   sscanf (argv[2],"%d", &N);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   transformeOndelette(ImgIn, cNomImgLue, nH, nW, ImgOut, cNomImgEcrite, 1);
	
  /*int diff;
  ImgOut[0] = 0;
  for (int i=0; i < nH; i++) {
   for (int j=1; j < nW; j++)
     {
     	if(j-1 >= 0) {
          diff = ( ImgIn[i*nW+j] - ImgIn[i*nW+(j-1)] ) + 128;
        } else if(i-1 >= 0) {
        	diff = ( ImgIn[i*nW+j] - ImgIn[(i-1)*nW+j] ) + 128;
        }
        if(diff < 0) diff = 0;
	    if(diff > 255) diff = 255;
	    ImgOut[i*nW+j] = diff;
     }
   }*/



   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

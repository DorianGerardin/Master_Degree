#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

void transformeOndelette(OCTET *ImgIn, char* cNomImgLue, int nH, int nW, 
                         OCTET *ImgOut, char* cNomImgEcrite, int N) {
  int nTaille = nH * nW;
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  int A, B, C, D, Q;

  Q = 256;

  if (N = 0) {
    //ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH*2, nW*2);
  } else {
    for (int i=0; i < nH; i+=2) {
     for (int j=0; j < nW; j+=2)
      {
        A = ImgIn[i*nW+j];
        B = ImgIn[i*nW+j+1];
        C = ImgIn[(i+1)*nW+j];
        D = ImgIn[(i+1)*nW+j+1];

        int BF = (int)floor((A+B+C+D)/4);
        int MFv = (int)floor((A+B-C-D)/2) + 128;
        int MFh = (int)floor((A-B+C-D)/2) + 128;
        int HF = A-B-C+D + 128;

        ImgOut[(i/2)*nW+(j/2)] = BF;
        ImgOut[(i/2)*nW+(j/2 + 256)] = MFv / (Q/2);
        ImgOut[(i/2 + 256)*nW+(j/2)] = MFh / (Q/2);
        ImgOut[(i/2 + 256)*nW+(j/2 + 256)] = HF / Q;
      }
   }
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   /*OCTET* ImgIn2 = ImgOut;
   transformeOndelette(ImgIn2, cNomImgEcrite, nH/2, nW/2, ImgOut, cNomImgEcrite, N-1, Ndepart);*/
  }
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, N;
  
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
   sscanf (argv[2],"%d", &N);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;

   transformeOndelette(ImgIn, cNomImgLue, nH, nW, ImgOut, cNomImgEcrite, 2);
	
   return 1;
}

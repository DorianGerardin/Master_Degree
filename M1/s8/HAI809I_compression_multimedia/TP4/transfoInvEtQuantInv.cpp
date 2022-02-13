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
  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

  int Q = 256;
  for (int i=0; i < nH; i+=2) {
     for (int j=0; j < nW; j+=2)
      {
        int BF = ImgIn[(i/2)*nW+(j/2)];
        int MFv = (ImgIn[(i/2)*nW+(j/2 + 256)]) * (Q/2) - 128;
        int MFh = (ImgIn[(i/2 + 256)*nW+(j/2)]) * (Q/2) - 128;
        int HF = (ImgIn[(i/2 + 256)*nW+(j/2 + 256)]) * (Q) - 128;

        int A = BF + MFv/2 + MFh/2 + HF/4;
        int B = BF + MFv/2 - MFh/2 - HF/4;
        int C = BF - MFv/2 + MFh/2 - HF/4;
        int D = BF - MFv/2 - MFh/2 + HF/4;

        A > 255 ? A = 255 : A < 0 ? A = 0 : A = A;
        B > 255 ? B = 255 : B < 0 ? B = 0 : B = B;
        C > 255 ? C = 255 : C < 0 ? C = 0 : C = C;
        D > 255 ? D = 255 : D < 0 ? D = 0 : D = D;

        ImgOut[i*nW+j] = A;
        ImgOut[i*nW+j+1] = B;
        ImgOut[(i+1)*nW+j] = C;
        ImgOut[(i+1)*nW+j+1] = D;
      }
   }
	 
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

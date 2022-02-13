#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  char cNomImgLue1[250], cNomImgLue2[250];
   int nH, nW, nTaille;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn ImageIn2\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
   sscanf (filename1,"%s",cNomImgLue1) ;
   sscanf (filename2,"%s",cNomImgLue2) ;

   OCTET *ImgIn1, *ImgIn2;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH, &nW);
   nTaille = nH * nW;
  allocation_tableau(ImgIn1, OCTET, nTaille);
  allocation_tableau(ImgIn2, OCTET, nTaille);
  lire_image_pgm(cNomImgLue1, ImgIn1, nH * nW);
  lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);

    double PSNR = 0.f;
    double EQM = 0.f;
    for (int i = 0; i < nTaille; i++)
    {
        EQM += sqrt(pow(ImgIn1[i] - ImgIn2[i], 2));
    }
    EQM /= nTaille;
    PSNR = 10 * log10(pow(255, 2) / EQM);
    printf("--- PSNR (pgm) : %f\n", PSNR);
     
   free(ImgIn1);
   free(ImgIn2);
   return 1;
}

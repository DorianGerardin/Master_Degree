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
  float ddpRef[256] = {0};
  int histoRef[256] = {0};
  float Fref[256] = {0};
  float Tref[256] = {0};

  float ddp[256] = {0};
  int histo[256] = {0};
  float F[256] = {0};
  float T[256] = {0};
  char cNomImgLue[250], cNomImgLue2[250], cNomImgEcrite[250];
   int nH, nW, nTaille;
   int nH2, nW2, nTaille2;
  
  if (argc != 4) 
     {
       printf("Usage: ImageInRef ImageIn2 ImageOut\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  string s3 = string(folder) + string(argv[3]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
  char* filename3 = stringToCharArray(s3);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgLue2);
   sscanf (filename3,"%s",cNomImgEcrite);

   OCTET *ImgInRef, *ImgIn2, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH2, &nW2);
   nTaille = nH * nW;
   nTaille2 = nH2 * nW2;
  
   allocation_tableau(ImgInRef, OCTET, nTaille);
   allocation_tableau(ImgIn2, OCTET, nTaille2);
   lire_image_pgm(cNomImgLue, ImgInRef, nH * nW);
   lire_image_pgm(cNomImgLue2, ImgIn2, nH2 * nW2);
   allocation_tableau(ImgOut, OCTET, nTaille2);

   for (int i=0; i < nTaille; i++) {
      histoRef[ImgInRef[i]]++;
   }

   for (int i=0; i < nTaille2; i++) {
      histo[ImgIn2[i]]++;
   }

   for (int i=0; i < 256; i++) {
      ddpRef[i] = (float)histoRef[i] / (float)nTaille;
   }

   for (int i=0; i < 256; i++) {
      ddp[i] = (float)histo[i] / (float)nTaille2;
   }

   for (int i = 0; i < 256; ++i)
   {
     for (int j = 0; j < i; ++j)
     {
       Fref[i] += ddpRef[j];
     }
   }

   for (int i = 0; i < 256; ++i)
   {
     for (int j = 0; j < i; ++j)
     {
       F[i] += ddp[j];
     }
   }

   /*for (int i = 0; i < 256; ++i)
   {
     T[i] = 255 * F[i];
     std::cout << Fref[i] << std::endl;
   }*/

   for (int i = 0; i < 256; ++i)
   {
     Tref[i] = 255 * Fref[i];
   }

   for (int i = 0; i < nTaille2; ++i)
   {
     ImgIn2[i] = T[(int)floor(ImgIn2[i])];
   }

   for (int i = 0; i < nTaille2; ++i)
   {
    //bool trouve = false;
     for (int j = 0; j < 255; ++j)
     {
       if((float)ImgIn2[i] / (float)255 >= Fref[j] && (float)ImgIn2[i] / (float)255 < Fref[j+1]) {
        ImgOut[i] = j;
        //trouve = true;
        break;
       }
     }
   }

   ecrire_image_pgm(cNomImgEcrite, ImgOut, nH2, nW2);
   free(ImgInRef);
   free(ImgIn2);
   return 1;
}

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
  int histoR[256] = {0};
  int histoG[256] = {0};
  int histoB[256] = {0};
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, nTaille3;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn ImageOut\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".ppm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;

   std::cout << cNomImgLue << endl;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
   nTaille3 = nH * nW * 3;
  
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);

   for (int i=0; i < nTaille3; i+=3) {
    histoR[ImgIn[i]]++;
    histoG[ImgIn[i+1]]++;
    histoB[ImgIn[i+2]]++;
   }

   bool aR0Trouve = false;
   int aR0 = 0;
   int aR1 = 0;

   bool aG0Trouve = false;
   int aG0 = 0;
   int aG1 = 0;

   bool aB0Trouve = false;
   int aB0 = 0;
   int aB1 = 0;

   for (int i = 0; i < 256; ++i)
   {
     if(histoR[i] > 0) {
      aR1 = i;
      if(!aR0Trouve) {
        aR0Trouve = true;
        aR0 = i;
      }
     }

     if(histoG[i] > 0) {
      aG1 = i;
      if(!aG0Trouve) {
        aG0Trouve = true;
        aG0 = i;
      }
     }

     if(histoB[i] > 0) {
      aB1 = i;
      if(!aB0Trouve) {
        aB0Trouve = true;
        aB0 = i;
      }
     }
   }

   std::cout << "aR0 : " << aR0 << " aR1 : " << aR1 << endl;
   std::cout << "aG0 : " << aG0 << " aG1 : " << aG1 << endl;
   std::cout << "aB0 : " << aB0 << " aB1 : " << aB1 << endl;

   float alphaR = -255 * aR0 / (float)(aR1-aR0);
   float betaR = 255 / (float)(aR1-aR0);

   float alphaG = -255 * aG0 / (float)(aG1-aG0);
   float betaG = 255 / (float)(aG1-aG0);

   float alphaB = -255 * aB0 / (float)(aB1-aB0);
   float betaB = 255 / (float)(aB1-aB0);

   std::cout << "alphaR : " << alphaR << " betaR : " << betaR << endl;
   std::cout << "alphaG : " << alphaG << " betaG : " << betaG << endl;
   std::cout << "alphaB : " << alphaB << " betaB : " << betaB << endl;

   for (int i=0; i < nTaille3; i++) {
    ImgOut[i] = alphaR + betaR*ImgIn[i];
    ImgOut[i+1] = alphaG + betaG*ImgIn[i+1];
    ImgOut[i+2] = alphaB + betaB*ImgIn[i+2];
   }

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

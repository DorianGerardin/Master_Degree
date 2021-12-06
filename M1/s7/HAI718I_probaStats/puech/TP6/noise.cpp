// blur.cpp : Floute une image en niveau de gris

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "image_ppm.h"
#include "file.h"
using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue1[250], cNomImgLue2[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  int histo[256] = {0};
  float moyenne = 0;
  float variance = 0;
  float ecartType = 0;
  int som;
  if (argc != 4) 
     {
       printf("Usage: ImageIn1 ImageIn2 flou ImageOut \n"); 
       exit (1) ;
     }

  const char *folder = "./img/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  string s3 = string(folder) + string(argv[3]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
  char* filename3 = stringToCharArray(s3);

  sscanf (filename1,"%s",cNomImgLue1) ;
  sscanf (filename2,"%s",cNomImgLue2);
  sscanf (filename3,"%s",cNomImgEcrite);

   OCTET *ImgIn1, *ImgIn2, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn1, OCTET, nTaille);
   allocation_tableau(ImgIn2, OCTET, nTaille);
   lire_image_pgm(cNomImgLue1, ImgIn1, nH * nW);
   lire_image_pgm(cNomImgLue2, ImgIn2, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   string const nomFichier("ddp_noise.dat");
   ofstream monFlux(nomFichier.c_str());
	

   for (int i=0; i < nH; i++)
	   for (int j=0; j < nW; j++)
	     {
			if (ImgIn1[i*nW+j]-ImgIn2[i*nW+j] < 127) {
				ImgOut[i*nW+j] = ImgIn1[i*nW+j]-ImgIn2[i*nW+j] + 128;
			} else {
				ImgOut[i*nW+j] = ImgIn1[i*nW+j]-ImgIn2[i*nW+j];
			}
	     }

	for (int i=0; i < nH; i++) {
	    for (int j=0; j < nW; j++)
	     {
	        histo[ImgOut[i*nW+j]]++;
	     }
	}

	if(monFlux)    
    {
      for (int i = 0; i < 256; ++i)
       {
         moyenne += histo[i] * i;
         monFlux << i << " " << (float)histo[i]/nTaille << endl;
       }
       moyenne /= nTaille;

       for (int i = 0; i < 256; ++i)
       {
         variance += histo[i] * pow((i - moyenne), 2);
       }

       //variance = variance / nTaille - moyenne;
       variance /= nTaille;
       ecartType = sqrt(variance);
       cout << "variance : " << variance << endl;
       cout << "ecartType : " << ecartType << endl;
       cout << "moyenne : " << moyenne << endl;
       cout << "distribGauss(x) = " << "(1 / " << ecartType << " * sqrt(2*" << M_PI << ")) * exp(-((x-" << moyenne << ")**2) / (2*" << ecartType << "**2))" << endl;

    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier histo.dat." << endl;
    }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn1);
   free(ImgIn2);
   free(ImgOut);
   return 1;
}
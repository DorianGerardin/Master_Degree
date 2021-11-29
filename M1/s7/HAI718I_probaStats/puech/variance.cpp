#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  int histo[256] = {0};
  float moyenne = 0;
  float variance = 0;
  float ecartType = 0;
  int nH, nW, nTaille;
  
  if (argc != 2) {
   printf("Usage: ImageIn namefile \n"); 
   exit (1) ;
  }

  const char *folder = "img/";
  const char *extension = ".pgm";
  string s = string(folder) + string(argv[1]) + string(extension);
  int filenameSize = s.length();
  char filename[filenameSize+1];
  strcpy(filename, s.c_str());
   
   sscanf (filename,"%s",cNomImgLue);
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;

   OCTET *ImgIn;
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

   string const nomFichier("ddp.dat");
   ofstream monFlux(nomFichier.c_str());

   for (int i=0; i < nH; i++) {
    for (int j=0; j < nW; j++)
     {
        histo[ImgIn[i*nW+j]]++;
     }
   }

   if(monFlux)    
    {
      for (int i = 0; i < 256; ++i)
       {
         moyenne += histo[i] * i;
       }
       moyenne /= nTaille;

       for (int i = 0; i < 256; ++i)
       {
         variance += histo[i] * pow((i - moyenne), 2);
         //variance += pow(histo[i], 2);
       }

       //variance = variance / nTaille - moyenne;
       variance /= nTaille;
       ecartType = sqrt(variance);
       cout << "variance : " << variance << endl;
       cout << "ecartType : " << ecartType << endl;
       cout << "moyenne : " << moyenne << endl;

    }
    else
    {
        cout << "ERREUR: Impossible d'ouvrir le fichier histo.dat." << endl;
    }
   
}

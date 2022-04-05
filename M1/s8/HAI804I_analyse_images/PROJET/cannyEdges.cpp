// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

struct Image {
  OCTET *data;
  OCTET *newData;
  char* filename;
  int size, nW, nH;
  double *GKernel[5];
  int GKernelSize;

  void getGaussianFilter(double GKernel[][5])
  {
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
 
    double GKernelSize = 0.0;
 
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            r = sqrt(x * x + y * y);
            GKernel[x + 2][y + 2] = (exp(-(r * r) / s)) / (M_PI * s);
            GKernelSize += GKernel[x + 2][y + 2];
        }
    }
 
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            GKernel[i][j] /= GKernelSize;
  }

  void applyFilter() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        int moyenne = GKernel[0][0];
        if(i-2 >= 0 && i+2 < nH && j-2 >= 0 && j+2 < nW) {
          for (int k = -2; k < 2; ++k)
          {
            for (int l = -2; l < 2; ++l) {
              moyenne += data[(i+k)*nW+(j+l)] * GKernel[k][l]; 
            }
          } newData[i*nW+j] = (int)floor(moyenne / GKernelSize);
        }
      }
    }
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, nTaille3;
  
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
   sscanf (filename2,"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   Image *imgIn;
   allocation_tableau(imgIn->data, OCTET, nTaille);
   allocation_tableau(imgIn->newData, OCTET, nTaille);

   imgIn->data = ImgIn;
   imgIn->newData = ImgIn;
   imgIn->filename = cNomImgLue;
   imgIn->size = nTaille;
   imgIn->nH = nH;
   imgIn->nW = nW;

   std::cout << "news data 0 : " << imgIn->newData[0] << std::endl;
   std::cout << "nH : " << imgIn->nH << std::endl;
   std::cout << "nW : " << imgIn->nW << std::endl;
   std::cout << "size : " << imgIn->size << std::endl;

   ecrire_image_pgm(cNomImgEcrite, imgIn->newData, imgIn->nH, imgIn->nW);
   free(ImgIn);
   free(ImgOut);
   return 1;
}

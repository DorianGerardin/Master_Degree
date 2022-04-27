// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include "image_ppm.h"
#include "file.h"

using namespace std;

struct Image {
  OCTET *data;
  OCTET *filteredData;
  OCTET *dataNoise;
  OCTET *out;
  char* filename;
  int histo[256] = {0};
  float ddp[256] = {0.};
  int size, nW, nH;
  double GKernel[5][5];
  double GKernelSize = 0.0;
  float seuil;

  void updateData() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        data[i*nW+j] = out[i*nW+j];
      }
    }
  }

  void getGaussianFilter()
  {
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
 
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            r = x * x + y * y;
            GKernel[x + 2][y + 2] = (exp(-(r / s))) / (M_PI * s);
            GKernelSize += GKernel[x + 2][y + 2];
        }
    }
 
    for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
          GKernel[i][j] /= GKernelSize;
      }
    }
  }


  void applyGaussianFilter() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        int moyenne = 0;
        if(i-2 >= 0 && i+2 < nH && j-2 >= 0 && j+2 < nW) {
          for (int k = -2; k <= 2; ++k)
          {
            for (int l = -2; l <= 2; ++l) {
              moyenne += (int)floor(data[(i+k)*nW+(j+l)] * GKernel[k+2][l+2]);
            }
          } filteredData[i*nW+j] = moyenne;
        } else filteredData[i*nW+j] = data[i*nW+j];
      }
    }
  }

  void blur() {
    for (int i=1; i < nH-1; i++) {
      for (int j=1; j < nW-1; j++)
       {
        int sum = data[(i-1)*nW+j-1]+data[(i-1)*nW+j]+data[(i-1)*nW+j+1]
                  + data[i*nW+j-1]+data[i*nW+j]+data[i*nW+j+1]
                  + data[(i+1)*nW+j-1]+data[(i+1)*nW+j]+data[(i+1)*nW+j+1];
        filteredData[i*nW+j]=sum/9;
      }
    }
  }

  void noise() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        if (filteredData[i*nW+j] - data[i*nW+j] < 127) {
          dataNoise[i*nW+j] = filteredData[i*nW+j] - data[i*nW+j] + 128;
        } else {
          dataNoise[i*nW+j] = filteredData[i*nW+j] - data[i*nW+j];
        }
      }
    }
  }

  void density() {
    for (int i=0; i < nH; i++) {
      for (int j=0; j < nW; j++) {
        histo[dataNoise[i*nW+j]]++;
      }
    }

    for (int i = 0; i < 256; ++i) {
      ddp[i] = (float)histo[i]/(float)size;
      cout << ddp[i] << endl;
    }
  }

  void detect() {
    for (int i = 0; i < nH; ++i){
      for (int j = 0; j < nW; ++j){
        int histoLocale[256] = {0};
        float ddpLocale[256] = {0.};

        if(i-5 >= 0 && i+5 < nH && j-5 >= 0 && j+5 < nW) {
          for (int k = -5; k <= 5; ++k){
            for (int l = -5; l <= 5; ++l) {
              histoLocale[dataNoise[(i+k)*nW+(j+l)]]++ ;
            }
          }

          for (int k = 0; k < 256; k++) {
            ddpLocale[k] = (float)histoLocale[k]/100.;
          }
          float sum = 0 ;
          float difference[256];
          for (int k = 0 ; k < 256 ; k++ ) {
            difference[k] = abs(ddp[k] - ddpLocale[k]);
            sum += difference[k];
          }
          float meanDiff = sum / 256.;
          cout << "meanDiff : " << meanDiff << endl; 
          if (meanDiff > seuil) {
            out[i*nW+j] = 255;
          } else {
            out[i*nW+j] = data[i*nW+j] ;
          }

        }
      }
    }
  }


};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;
  float seuil; 
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn ImageOut seuil\n"); 
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
  sscanf (argv[3],"%f",&seuil);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = cNomImgLue;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
    img->seuil = seuil;
  };

  allocation_tableau(img->data, OCTET, img->size);
  allocation_tableau(img->filteredData, OCTET, img->size);
  allocation_tableau(img->dataNoise, OCTET, img->size);
  allocation_tableau(img->out, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, nH * nW);

/*  img->getGaussianFilter();
  img->applyGaussianFilter();*/
  img->blur();
  img->noise();
  // img->density();
  // img->detect();

  ecrire_image_pgm(cNomImgEcrite, img->dataNoise, img->nH, img->nW);
  free(img->data);
  free(img->out);
  free(img->filteredData);
  return 1;
}

// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <climits>  
#include <algorithm> 
#include <vector>
#include <array>
#include <cmath>
#include <cfloat>
#include "image_ppm.h"
#include "file.h"

using namespace std;

void getGaussianFilter(double GKernel[][3])
  {
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
    double GKernelSize = 0.0;

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            r = x * x + y * y;
            GKernel[x + 1][y + 1] = (exp(-(r / s))) / (M_PI * s);
            GKernelSize += GKernel[x + 1][y + 1];
        }
    }
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
          GKernel[i][j] /= GKernelSize;
      }
    }
  }

struct Image {
  OCTET *data;
  OCTET *out;
  char* filename;
  char* filenameOut;
  int size, nW, nH;

  void updateData() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        data[i*nW+j] = out[i*nW+j];
      }
    }
  }

  void applyFilter(double kernel[3][3]) {
    allocation_tableau(out, OCTET, size);

    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        int moyenne = 0;
        if(i-1 >= 0 && i+1 < nH && j-1 >= 0 && j+1 < nW) {
          for (int k = -1; k <= 1; ++k)
          {
            for (int l = -1; l <= 1; ++l) {
              moyenne += (int)floor(data[(i+k)*nW+(j+l)] * kernel[k+1][l+1]);
            }
            if(moyenne < 0) moyenne = 0;
          } out[i*nW+j] = moyenne;
        } else out[i*nW+j] = 0;
      }
    }
    //ecrire_image_pgm(filenameOut, out, nH, nW);
    updateData();
    free(out);
  }

  void downSample(int length, int sample) {
    allocation_tableau(out, OCTET, nH/sample * nW/sample);
    for(int i = 0; i < nH; i+=length) {
      for(int j = 0; j < nW; j+=length) {
        vector<int> tempArray;
        for (int i2 = i; i2 < i+length; i2++){
          for (int j2 = j; j2 < j+length; j2++){
            tempArray.push_back(data[i2*nW+j2]);
          }
        }
        int maxValue = *max_element(tempArray.begin(), tempArray.end());
        out[i/sample*nW/sample+j/sample] = maxValue;
      }
    }
    //ecrire_image_pgm(filenameOut, out, nH/2, nW/2);
    updateData();
    //free(out);
  }

  vector<Image*> createImages(Image *baseImg, int nbImages) {
    vector<Image*> imgArray;
    for (int j = 0; j < nbImages; ++j) {
      Image *img = new Image();
      {
        img->size = baseImg->size;
        img->nH = baseImg->nH;
        img->nW = baseImg->nW;
      };
      allocation_tableau(img->data, OCTET, img->size);
      for (int i = 0; i < baseImg->size; ++i) img->data[i] = baseImg->data[i];
      imgArray.push_back(img);
    }
    return imgArray;
  }

  void CNN(int nbLayers) {
    int nbFilters = 5;
    double d = 1./9.;
    double gaussianFilter[3][3]; getGaussianFilter(gaussianFilter);
    double filters[5][3][3] = {
      {{1.,0.,-1.}, {2.,0.,-2.}, {1.,0.,-1.}},  // passe haut (Sobel H)
      {{1.,2.,1.}, {0.,0.,0.}, {-1.,-2.,-1.}},  // passe haut (Sobel V)
      {{0.0751136, 0.123841, 0.0751136}, {0.123841, 0.20418, 0.123841}, {0.0751136, 0.123841, 0.0751136}}, // passe bas (gaussian)
      {{d,d,d}, {d,d,d}, {d,d,d}},              // passe bas (blur)
      {{0.,1.,0.}, {1.,-4.,1.}, {0.,1.,0.}}     // filtre autre
    };

    int sample = 2;
    int imgSize = nH;
    vector<Image*> newImages;

    for (int i = 0; i < nbLayers; ++i) {
      if(i == 0) {
        newImages = createImages(this, nbFilters);
        for (int j = 0; j < newImages.size(); ++j) {
          newImages[j]->applyFilter(filters[j]);
          newImages[j]->downSample(2, sample);
        }
      }
      else {
        vector<Image*> tempNewImages;
        for (int j = 0; j < newImages.size(); ++j) {
          vector<Image*> newFiltersImgs;
          newFiltersImgs = createImages(newImages[j], nbFilters);
          for (int k = 0; k < newFiltersImgs.size(); ++k) {
            newFiltersImgs[k]->applyFilter(filters[k]);
            newFiltersImgs[k]->downSample(2, sample);
            tempNewImages.push_back(newFiltersImgs[k]);
          }
        }
        newImages = tempNewImages;
      }
      imgSize /= sample;
    }

    const char *folder = "./test/";
    const char *extension = ".pgm";
    for (int i = 0; i < newImages.size(); ++i)
    {
      string s = string(folder) + to_string(i) + string(extension);
      char* filename = stringToCharArray(s);
      ecrire_image_pgm(filename, newImages[i]->out, imgSize, imgSize);
      free(filename);
    }

    for (int j = 0; j < newImages.size(); ++j) free(newImages[j]);
  }

};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250], mode[100];
  int nH, nW, nTaille; 
  unsigned int key;

  if (argc != 3) 
  {
     printf("Usage: ImageIn ImageOut\n"); 
     exit (1) ;
  }

  const char *folder = "./boar/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
  sscanf (filename1,"%s", cNomImgLue);
  sscanf (filename2,"%s", cNomImgEcrite);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = cNomImgLue;
    img->filenameOut = cNomImgEcrite;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
  };

  allocation_tableau(img->data, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, img->size);

  img->CNN(3);

  free(img->data);
  free(filename1);
  free(filename2);
  return 1;
}

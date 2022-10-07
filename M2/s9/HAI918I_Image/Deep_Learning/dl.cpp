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
#include <cmath>
#include <cfloat>
#include "image_ppm.h"
#include "file.h"

using namespace std;


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
    //free(out);
  }

  void downSample(int length) {
    allocation_tableau(out, OCTET, nH/2 * nW/2);
    for(int i = 0; i < nH; i+=length) {
      for(int j = 0; j < nW; j+=length) {
        vector<int> tempArray;
        for (int i2 = i; i2 < i+length; i2++){
          for (int j2 = j; j2 < j+length; j2++){
            tempArray.push_back(data[i2*nW+j2]);
          }
        }
        int maxValue = *max_element(tempArray.begin(), tempArray.end());
        out[i/2*nW/2+j/2] = maxValue;
      }
    }
    //ecrire_image_pgm(filenameOut, out, nH/2, nW/2);
    updateData();
    //free(out);
  }

  void CNN(int nbIteration, int nbFilter) {
    double passeHaut[3][3] = {{-1.,0.,1.}, {-1.,0.,1.}, {-1.,0.,1.}};
    double passeBas[3][3] = {{4.,-1.,-3.}, {2.,0.,-1.}, {1.,-2.,0.}};

    vector<Image*> imgArray;
    for (int j = 0; j < nbFilter; ++j) {
      Image *img = new Image();
      {
        img->size = nTaille;
        img->nH = nH;
        img->nW = nW;
      };
      allocation_tableau(img->data, OCTET, img->size);
      imgArray.push_back(img);
    }
    
    for (int i = 0; i < nbIteration; ++i) {
      for (int j = 0; j < nbFilter; ++j) {
        if(j%2 == 0) {
          imgArray[j]->applyFilter(passeHaut);
        } else {
          imgArray[j]->applyFilter(passeBas);
        }
      }
      for (int j = 0; j < imgArray.size(); ++j){
        imgArray[i]->downSample(2);
      }
    }
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
  lire_image_pgm(img->filename, img->data, img->size );

  double kernel[3][3] = {{0.,1.,0.}, {1.,-4.,1.}, {0.,1.,0.}};
  double passeHaut[3][3] = {{-1.,0.,1.}, {-1.,0.,1.}, {-1.,0.,1.}};
  double passeBas[3][3] = {{4.,-1.,-3.}, {2.,0.,-1.}, {1.,-2.,0.}};
  img->applyFilter(passeBas);


  //img->downSample(2);

  free(img->data);
  free(filename1);
  free(filename2);
  return 1;
}

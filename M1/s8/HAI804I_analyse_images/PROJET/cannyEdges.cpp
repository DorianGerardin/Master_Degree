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
  OCTET *defaultData;
  OCTET *data;
  OCTET *out;
  char* filename;
  int size, nW, nH;
  double GKernel[5][5];
  double GKernelSize = 0.0;
  vector<float> gradientAngles;
  int SB, SH;

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
          } out[i*nW+j] = moyenne;
        } else out[i*nW+j] = data[i*nW+j];
      }
    }
    updateData();
  }

  void applyGradient() {
    gradientAngles.resize(size);
    int sobelH[3][3] = {{1,0,-1}, {2,0,-2}, {1,0,-1}};
    int sobelV[3][3] = {{1,2,1}, {0,0,0}, {-1,-2,-1}};
    for (int i = 0; i < nH; ++i) {
     for (int j = 0; j < nW; ++j) {
      int Gh = 0, Gv = 0;
      if(i == 0 || j == 0 || i == nH-1 || j == nW-1) out[i*nW+j] = 0;
      else if(i == 1 || j == 1 || i == nH-2 || j == nW-2) out[i*nW+j] = 0;
      else if(i-1 >= 0 && i+1 < nH && j-1 >= 0 && j+1 < nW) {
        for (int k = -1; k <= 1; ++k) {
          for (int l = -1; l <= 1; ++l) {
            Gh += data[(i+k)*nW+(j+l)] * sobelH[k+1][l+1];
            Gv += data[(i+k)*nW+(j+l)] * sobelV[k+1][l+1];      
          }
        }
      }
      float phaseG = std::atan((float)Gh/(float)Gv);
      if (phaseG < 0) phaseG += 180;
      gradientAngles[i*nW+j] = phaseG;
      out[i*nW+j] = min((int)floor(sqrt(pow(Gh, 2) + pow(Gv, 2))), 255);
     }
   }
   updateData();
  }

  void nonMaxSuppression() {
    for (int i = 0; i < nH; ++i) {
     for (int j = 0; j < nW; ++j) {
      float phaseG = gradientAngles[i*nW+j];
      //cout << "phaseG suite : "  << i*nW+j  << " : " << phaseG << endl;
      int n1 = 255, n2 = 255;
      //angle 0
      if ((0 <= phaseG < 22.5) || (157.5 <= phaseG <= 180)) {
        n1 = data[(i+1)*nW+j];
        n2 = data[(i-1)*nW+j];
      }
      //angle 45
      else if (22.5 <= phaseG < 67.5) {
        n1 = data[(i+1)*nW+j+1];
        n2 = data[(i-1)*nW+j-1];
      }
      //angle 90
      else if (67.5 <= phaseG < 112.5) {
        n1 = data[i*nW+j+1];
        n2 = data[i*nW+j+1];
      }
      //angle 135
      else if (112.5 <= phaseG < 157.5) {
        n1 = data[(i-1)*nW+j+1];
        n2 = data[(i+1)*nW+j-1];
      }
      if (data[i*nW+j] >= n1 && data[i*nW+j] >= n2) {
        out[i*nW+j] = data[i*nW+j];
      }
      else out[i*nW+j] = 0;
      }
    }
  updateData();
  }

  void doubleTreshold() {
    for (int i=0; i < nH; i++) {
      for (int j=0; j < nW; j++) {
        if(i == 0 || j == 0 || i == nH-1 || j == nW-1) out[i*nW+j] = 0;
        if(data[i*nW+j] < SH) out[i*nW+j] = data[i*nW+j];
        else out[i*nW+j] = 255;
      }
    }
    updateData();
  }

  void hysteresis() {
    for (int i = 0; i < nH; i++) {
      for (int j = 0; j < nW; j++) {
        if(SB < data[i*nW+j]) {
          // Si l'un des contours est blanc
          if(i-1 >= 0 && j-1 >= 0 && data[(i-1)*nW+(j-1)] == 255) 
                out[i*nW+j] = 255; // p(i-1,j-1)
          else if(j-1 >= 0 && data[i*nW+(j-1)] == 255) 
                out[i*nW+j] = 255; // p(i,j-1)
          else if(i+1 < nH && j-1 >= 0 && data[(i+1)*nW+(j-1)] == 255) 
                out[i*nW+j] = 255; // p(i+1,j-1)
          else if(i-1 >= 0 && data[(i-1)*nW+j] == 255) 
                out[i*nW+j] = 255; // p(i-1,j)
          else if(i+1 < nH && data[(i+1)*nW+j] == 255) 
                out[i*nW+j] = 255; // p(i+1,j)
          else if(i-1 >= 0 && j+1 < nW && data[(i-1)*nW+(j+1)] == 255) 
                out[i*nW+j] = 255; // p(i-1,j+1)
          else if(j+1 < nW && data[i*nW+(j+1)] == 255) 
                out[i*nW+j] = 255; // p(i,j+1)
          else if(i+1 < nH && j+1<nW && data[(i+1)*nW+(j+1)] == 255) 
                out[i*nW+j] = 255; // p(i+1,j+1)
          // Sinon pixel noir
          else out[i*nW+j] = 0;
        }
        else out[i*nW+j] = 0;
      }
    }
    updateData();
  }
};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, SB, SH; 
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn ImageOut SB SH\n"); 
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
  sscanf (argv[3],"%d",&SB) ;
  sscanf (argv[4],"%d",&SH) ;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = cNomImgLue;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
    img->SB = SB;
    img->SH = SH;
  };

  allocation_tableau(img->data, OCTET, img->size);
  allocation_tableau(img->defaultData, OCTET, img->size);
  allocation_tableau(img->out, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, nH * nW);

  img->getGaussianFilter();
  img->applyGaussianFilter();
  img->applyGradient();
  img->nonMaxSuppression();
  img->doubleTreshold();
  img->hysteresis();

  ecrire_image_pgm(cNomImgEcrite, img->out, img->nH, img->nW);
  free(img->data);
  free(img->out);
  free(img->defaultData);
  return 1;
}

// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <climits>  
#include <algorithm> 
#include <vector>
#include "image_ppm.h"
#include "file.h"

using namespace std;

struct Image {
  OCTET *cannyEdges;
  OCTET *data;
  OCTET *out;
  char* filename;
  int size, nW, nH;

  void updateData() {
    for(int i = 0; i < nH; i++) {
      for(int j = 0; j < nW; j++) {
        data[i*nW+j] = out[i*nW+j];
      }
    }
  }

  void impaiting() {
    int patchSize = 3;
    int demiPatchSize = patchSize/2;
    int offset = 1;
    for(int i = demiPatchSize; i < nH - demiPatchSize; i+=patchSize) {
      for(int j = demiPatchSize; j < nW - demiPatchSize; j+=patchSize) {
        if(data[(i-1)*nW+j-1] == 0 || data[(i-1)*nW+j] == 0 || data[(i-1)*nW+j+1] == 0
                  || data[i*nW+j-1] == 0 || data[i*nW+j] == 0 || data[i*nW+j+1] == 0
                  || data[(i+1)*nW+j-1] == 0 || data[(i+1)*nW+j] == 0 || data[(i+1)*nW+j+1] == 0) {
          vector<int> distances(4,3);
          //haut
          for(int t = i-2; t >= 0; t--) {
            distances[0]++;
            if(data[t*nW+j] != 0) break;
            else if(cannyEdges[t*nW+j] == 255) {
              distances[0] = INT_MAX;
              break;
            }
          }

          //bas
          for(int b = i+2; b < nH; b++) {
            distances[1]++;
            if(data[b*nW+j] != 0) break;
            else if(cannyEdges[b*nW+j] == 255) {
              distances[1] = INT_MAX;
              break;
            }
          }

          //gauche
          for(int l = j-2; l >= 0; l--) {
            distances[2]++;
            if(data[(i+1)*nW+l] != 0) break;
            else if(cannyEdges[i*nW+l] == 255) {
              distances[2] = INT_MAX;
              break;
            }
          }

          //droites
          for(int r = j+2; r < nW; r++) {
            distances[3]++;
            if(data[(i+1)*nW+r] != 0) break;
            else if(cannyEdges[i*nW+r] == 255) {
              distances[3] = INT_MAX;
              break;
            }
          }
          
          int minDistance = INT_MAX;
          for (int c = 0; c < 4; c++) {
            if(minDistance > distances[c]) minDistance = distances[c];
              
          }

          int direction;
          for (int d = 0; d < 4; ++d) {
            if(minDistance == distances[d]) direction = d;
          }

          vector<int> patch;

          //haut
          if(direction == 0) {
            vector<int> randomsValueI = { 0, 1, 2 };
            vector<int> randomsValueJ = { 0, -1, 1, -2, 2 };

            int randomI = randomsValueI[rand() % randomsValueI.size()];
            int randomJ = randomsValueJ[rand() % randomsValueJ.size()];

            int patchCenterI = (i <= patchSize+1) ? i : i-(minDistance+offset+randomI);
            int patchCenterJ = (j <= patchSize+1 || j >= nW-patchSize-1) ? j : j+randomJ;

            if(patchCenterI-1 >= 0 && patchCenterI+1 < nH && patchCenterJ-1 >= 0 && patchCenterJ+1 < nW) {
              for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
                for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
                  patch.push_back(data[(patchCenterI+k)*nW+patchCenterJ+l]);
                }
              }
            } 
          }
          //bas
          if(direction == 1) {
            vector<int> randomsValueI = { 0, 1, 2 };
            vector<int> randomsValueJ = { 0, -1, 1, -2, 2 };

            int randomI = randomsValueI[rand() % randomsValueI.size()];
            int randomJ = randomsValueJ[rand() % randomsValueJ.size()];

            int patchCenterI = (i >= nH-patchSize-1) ? i : i+(minDistance+offset+randomI);
            int patchCenterJ = (j == 1 || j >= nW-patchSize-1) ? j : j+randomJ;

            if(patchCenterI-1 >= 0 && patchCenterI+1 < nH && patchCenterJ-1 >= 0 && patchCenterJ+1 < nW) {
              for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
                for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
                  patch.push_back(data[(patchCenterI+k)*nW+patchCenterJ+l]);
                }
              }
            }
          }
          //gauche
          if(direction == 2) {
            vector<int> randomsValueI = { 0, -1, 1, -2, 2 };
            vector<int> randomsValueJ = { 0, 1, 2 };

            int randomI = randomsValueI[rand() % randomsValueI.size()];
            int randomJ = randomsValueJ[rand() % randomsValueJ.size()];

            int patchCenterI = (i == 1 || i >= nH-patchSize-1) ? i : i+randomI;
            int patchCenterJ = (j <= patchSize+2) ? j : j-(minDistance+offset+randomJ);

            if(patchCenterI-1 >= 0 && patchCenterI+1 < nH && patchCenterJ-1 >= 0 && patchCenterJ+1 < nW) {
              for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
                for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
                  patch.push_back(data[(patchCenterI+k)*nW+patchCenterJ+l]);
                }
              }
            }
          }
          //droite
          if(direction == 3) {
            vector<int> randomsValueI = { 0, -1, 1, -2, 2 };
            vector<int> randomsValueJ = { 0, 1, 2 };

            int randomI = randomsValueI[rand() % randomsValueI.size()];
            int randomJ = randomsValueJ[rand() % randomsValueJ.size()];

            int patchCenterI = (i == 1 || i >= nH-patchSize-1) ? i : i+randomI;
            int patchCenterJ = (j >= nW-patchSize-4) ? j : j+(minDistance+offset+randomJ);

            if(patchCenterI-1 >= 0 && patchCenterI+1 < nH && patchCenterJ-1 >= 0 && patchCenterJ+1 < nW) {
              for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
                for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
                  patch.push_back(data[(patchCenterI+k)*nW+patchCenterJ+l]);
                }
              }
            }
          }

          int voisinIndex = 0;
          if(i-demiPatchSize >= 0 && i+demiPatchSize < nH && j-demiPatchSize >= 0 && j+demiPatchSize < nW) {
            //cout << "je remplis" << endl;
            for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
              for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
                out[(i+k)*nW+j+l] = patch[voisinIndex];
                voisinIndex++;
              }
            }
          } else out[i*nW+j] = data[i*nW+j];
        } else {
          for (int k = -(patchSize/2); k <= (patchSize/2); ++k) {
            for (int l = -(patchSize/2); l <= (patchSize/2); ++l) {
              out[(i+k)*nW+j+l] = data[(i+k)*nW+j+l];
            }
          }
        }
      }
    }
    updateData();
  }
};

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cannyEdges[250], cNomImgEcrite[250];
  int nH, nW, nTaille; 
  
  if (argc != 4) 
     {
       printf("Usage: CannyEdges ImageIn ImageOut\n"); 
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
   
  sscanf (filename1,"%s",cannyEdges) ;
  sscanf (filename2,"%s",cNomImgLue);
  sscanf (filename3,"%s",cNomImgEcrite);

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;

  Image *img = new Image();
  {
    img->filename = cNomImgLue;
    img->size = nTaille;
    img->nH = nH;
    img->nW = nW;
  };

  allocation_tableau(img->data, OCTET, img->size);
  allocation_tableau(img->cannyEdges, OCTET, img->size);
  allocation_tableau(img->out, OCTET, img->size);
  lire_image_pgm(img->filename, img->data, nH * nW);
  lire_image_pgm(filename1, img->cannyEdges, nH * nW);

  img->impaiting();
  img->impaiting();
  img->impaiting();

  ecrire_image_pgm(cNomImgEcrite, img->out, img->nH, img->nW);
  free(img->data);
  free(img->cannyEdges);
  free(img->out);
  return 1;
}

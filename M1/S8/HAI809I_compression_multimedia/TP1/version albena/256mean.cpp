// test_grey.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include <math.h>
//#include <cmath>
//#include <iostream>
#include <stdlib.h>
#include "image_ppm.h"

int nH, nW;

double calculDistance(int *moyenne, int pixel, OCTET *ImgIn ){
  int r = (moyenne[0] - ImgIn[pixel]) * (moyenne[0] - ImgIn[pixel]);
  int g = (moyenne[1] - ImgIn[pixel+1]) * (moyenne[1] - ImgIn[pixel+1]);
  int b = (moyenne[2] - ImgIn[pixel+2]) * (moyenne[2] - ImgIn[pixel+2]) ;
  return sqrtf((double)(r+g+b));
} 

void calculMoyenne(int classe, OCTET *classes,  OCTET *ImgIn, int *moyenne){
  int nb = 0;
  int accum[3] = {0, 0, 0};
  for (int i=0; i < nH; i++)
    for (int j=0; j < nW; j+=3)
      if (classes[i*nW+j] == classe){
          accum[0] += ImgIn[i*nW+j];
          accum[1] += ImgIn[i*nW+j + 1];
          accum[2] += ImgIn[i*nW+j + 2];
          nb++;
      }
  if (nb != 0){
      //printf("\n\n\n29 %d\n",  nb);
      moyenne[0] = (int)ceil(accum[0] / nb);
      moyenne[1] = (int)ceil(accum[1] / nb);
      moyenne[2] = (int)ceil(accum[2] / nb);
  }
  //printf("\n\n\n33 \n");
} 



int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nTaille, direction, mean;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.ppm ImageOut.ppm mean\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcrite) ;
   sscanf (argv[3],"%d", &mean) ;

   OCTET *ImgIn, *ImgOut, *classes;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
   int nTaille3 = nTaille * 3;
    
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(classes, OCTET, nTaille3);
   allocation_tableau(ImgOut, OCTET, nTaille3);

  int v ;
  int moyenne[mean][3];
  for (int i=0; i < mean; i++){
    v = rand() % nTaille3;
    for (int j=0; j < 3; j++)
      moyenne[i][j] = ImgIn[v+j];
  }



    for (int i=0; i < nTaille3; i++){
      classes[i] = 0;
    }
  
   int changementsDeClasse = 10;
   int min = 5, pixel, classe;
   float distances[mean], d, dtmp;
   while (changementsDeClasse > min){
      changementsDeClasse = 0;
      for (int i=0; i < nTaille3; i+=3){
            d = calculDistance(moyenne[0], i, ImgIn );
            classe = 0;
            for (int j=1; j < mean; j++){
              dtmp = calculDistance(moyenne[j], i, ImgIn );
              if (d > dtmp){
                d = dtmp;
                classe = j;
              }
            }
            /* d1 = calculDistance(c1moyenne, i, ImgIn );
            d2 = calculDistance(c2moyenne, i, ImgIn ); */

            if (classes[i] != classe){
                classes[i] = classe;
                changementsDeClasse++;
              }

            } 

      for (int i=0; i < mean; i++){
        calculMoyenne(i, classes,  ImgIn ,moyenne[i]);
      }
      
      
    }

   

    for (int i=0; i < nTaille3; i+=3){
      ImgOut[i] = moyenne[classes[i]][0];
      ImgOut[i+1] = moyenne[classes[i]][1];
      ImgOut[i+2] =  moyenne[classes[i]][2];
    }
/*       if (classes[i] == 1){
        ImgOut[i] = c1moyenne[0];
        ImgOut[i+1] = c1moyenne[1];
        ImgOut[i+2] = c1moyenne[2];
      }
      else{
        ImgOut[i] = c2moyenne[0];
        ImgOut[i+1] = c2moyenne[1];
        ImgOut[i+2] = c2moyenne[2];
      } */
  
  

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}
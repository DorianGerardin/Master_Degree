#include <stdio.h>
#include <math.h> 
#include "image_ppm.h"

#include <iostream>

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille;

  if (argc != 3) 
  {
       printf("Usage: ImageIn.pgm ImageOut.pgm\n"); 
       exit (1) ;
  }
   
  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%s",cNomImgEcrite);


  OCTET *ImgIn, *ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
  allocation_tableau(ImgOut, OCTET, nTaille);

  int gH, gV;
  float gNorme;
  float D;

  for(int i = 0; i < nTaille; i++)
    ImgIn[i] -= 128;

  for (int i=0; i < nH; i++){
    for (int j=0; j < nW; j++){
      if( i == nH-1){
        gH = 0;
      }
      else{
        gH = ImgIn[(i+1) * nW + j] - ImgIn[i * nW + j];
      }

      if( j == nW-1){
        gV = 0;
      }
      else{
        gV = ImgIn[(i * nW + j + 1)] - ImgIn[i * nW + j];
      }

      gNorme = fmax(abs(gH),abs(gV));
      if(gV != 0){
        D = atan(abs(gH) / abs(gV));
      } else D = 0;
      

      float minim = D;
      for(float incr = -M_PI; incr < M_PI; incr += M_PI/4){
        if(minim < D / incr){
          minim = incr;
        }
      }

      if( minim == -M_PI){
        if(ImgIn[i * nW + j] * ImgIn[i * nW + j-1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if( minim ==-(3 * M_PI)/4 ){
        if(ImgIn[i * nW + j] * ImgIn[(i-1) * nW + j-1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      } 
      else if(minim == -M_PI/2){
        if(ImgIn[i * nW + j] * ImgIn[(i-1) * nW + j] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if(minim == -M_PI/4){
        if(ImgIn[i * nW + j] * ImgIn[(i+1) * nW + j+1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if(minim == 0){
        if(ImgIn[i * nW + j] * ImgIn[i * nW + j+1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if(minim == M_PI/2){
        if(ImgIn[i * nW + j] * ImgIn[(i-1) * nW + j+1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if(minim == (3 * M_PI)/4 ){
        if(ImgIn[i * nW + j] * ImgIn[(i-1) * nW + j] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      else if(minim == M_PI){
        if(ImgIn[i * nW + j] * ImgIn[(i-1) * nW + j-1] <= 0){
          ImgOut[i * nW + j] = gNorme;
        }else ImgOut[i * nW + j] = 0;
      }
      std::cout <<  (int)ImgOut[i * nW + j]<< std::endl;
    }
  }
  
  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn);
  return 1;
}
  
  
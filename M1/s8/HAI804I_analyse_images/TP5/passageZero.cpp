// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"
#include <math.h>
#include <cfloat>
#include <cmath>

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

   int Gh, Gv;
   float normeG, phaseG;
   for(int i = 0; i < nH; i++) {
   		for(int j = 0; j < nW; j++) {
   				ImgIn[i*nW+j] = ImgIn[i*nW+j] - 128;
   			}
   	}

   	for(int i = 0; i < nH; i++) {
   		for(int j = 0; j < nH; j++) {
 				if(i+1 <= nH-1) Gh = ImgIn[(i+1)*nW+j] - ImgIn[i*nW+j];
	     	else Gh = 0;
	     	if(j+1 <= nW-1) Gv = ImgIn[i*nW+j+1] - ImgIn[i*nW+j];
	     	else Gv = 0;
	     	normeG = max(abs(Gh), abs(Gv));
	     	if(Gv != 0){
        	phaseG = std::atan((float)Gh/(float)Gv);
      	} else phaseG = 0;

	     	float min = FLT_MAX;
	     	float angle = phaseG;
	     	int indexVoisin = 0;
	     	for (float i = -M_PI; i < M_PI; i+=M_PI/4)
	     	{
	     		float difference = abs(i-phaseG);
	     		std::cout << difference << std::endl;
	     		if (difference < min) {
	     			min = difference;
	     			angle = i;
	     		}
	     	}
	     	if(angle <= -M_PI + 0.1 && angle >= -M_PI - 0.1)                  	indexVoisin = i*nW+(j-1);
	     	else if(angle <= -(3*M_PI)/4 + 0.1 && angle >= -(3*M_PI)/4 - 0.1) 	indexVoisin = (i+1)*nW+(j-1);
	     	else if(angle <= -M_PI/2 + 0.1 && angle >= -M_PI/2 + 0.1)         	indexVoisin = (i+1)*nW+j;
	     	else if(angle <= -M_PI/4 + 0.1 && angle >= -M_PI/4 - 0.1)         	indexVoisin = (i+1)*nW+j+1;
	     	else if(angle <= 0.1 && angle >= -0.1) 													  	indexVoisin = i*nW+(j+1);
	     	else if(angle <= M_PI/4 + 0.1 && angle >= M_PI/4 - 0.1) 						indexVoisin = (i-1)*nW+(j+1);
	     	else if(angle <= M_PI/2 + 0.1 && angle >= M_PI/2 - 0.1) 						indexVoisin = (i-1)*nW+j;
	     	else if(angle <= (3*M_PI)/4 + 0.1 && angle >= (3*M_PI)/4 - 0.1)   	indexVoisin = (i-1)*nW+(j-1);

	     	if(ImgIn[i*nW+j] * ImgIn[indexVoisin] < 0) ImgOut[i*nW+j] = (int)normeG;
	     	else ImgOut[i*nW+j] = 0;
   		}
	  }
  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

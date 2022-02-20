// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

OCTET *ImgIn, *ImgOut;

int variance(OCTET *ImgIn, int minI, int maxI, int minJ, int maxJ, int nH, int nW) {
	int moyenne = 0;
	int variance = 0;
	for(int i = minI; i < maxI; i++) {
 		for(int j = minJ; j < maxJ; j++) { 
 			moyenne += ImgIn[i*nW+j];
  	}
	} moyenne /= (maxI-minI)*(maxJ-minJ);

	for(int i = minI; i < maxI; i++) {
 		for(int j = minJ; j < maxJ; j++) { 
 			variance += pow(ImgIn[i*nW+j] - moyenne, 2);
   		ImgOut[i*nW+j] = moyenne;
  	}
	} variance /= (maxI-minI)*(maxJ-minJ);
	return variance;
}

void divise(OCTET *ImgIn, int startH, int startW, int nH, int nW, int nHmax, int nWmax, int seuil) {

  float variance1 = 0;
  float variance2 = 0;
  float variance3 = 0;
  float variance4 = 0;

	if(nH <= 8 || nW <= 8) {
		//nothing
	} else {
		//haut gauche
		variance1 = variance(ImgIn, startH, startH+(nH/2), startW, startW+nW/2, nHmax, nWmax);
		if(variance1 > seuil) {
			divise(ImgIn, startH, startW, nH/2, nW/2, nHmax, nWmax, seuil);
		}

		//haut droit
		variance2 = variance(ImgIn, startH, startH+nH/2, startW+nW/2, startW+nW, nHmax, nWmax);
		if(variance2 > seuil) {
			divise(ImgIn, startH, startW+nW/2, nH/2, nW/2, nHmax, nWmax, seuil);
		}

		//bas gauche
		variance3 = variance(ImgIn, startH+(nH/2), startH+nH, startW, startW+nW/2, nHmax, nWmax);
		if(variance3 > seuil) {
			divise(ImgIn, startH+nH/2, startW, nH/2, nW/2, nHmax, nWmax, seuil);
		}

		//bas droit
		variance4 = variance(ImgIn, startH+(nH/2), startH+nH, startW+nW/2, startW+nW, nHmax, nWmax);
		if(variance4 > seuil) {
			divise(ImgIn, startH+(nH/2), startW+nW/2, nH/2, nW/2, nHmax, nWmax, seuil);
		}
	}
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nTaille3, seuil;
  
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
   sscanf (argv[3],"%d", &seuil);
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   divise(ImgIn, 0, 0, nH, nW, nH, nW, seuil);
  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

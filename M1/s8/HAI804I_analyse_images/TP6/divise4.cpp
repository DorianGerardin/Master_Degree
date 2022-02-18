// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nTaille3;
  float moyenne1 = 0;
  float moyenne2 = 0;
  float moyenne3 = 0;
  float moyenne4 = 0;

  float variance1 = 0;
  float variance2 = 0;
  float variance3 = 0;
  float variance4 = 0;

  
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


   //haut gauche
   for(int i = 0; i < nH/2; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			moyenne1 += ImgIn[i*nW+j];
	  	}
		} moyenne1 /= nTaille/4;

		for(int i = 0; i < nH/2; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			variance1 += pow(ImgIn[i*nW+j] - moyenne1, 2);
   			ImgOut[i*nW+j] = moyenne1;
	  	}
		}
		variance1 /= nTaille/4;


		//haut droit
		for(int i = 0; i < nH/2; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			moyenne2 += ImgIn[i*nW+j];
	  	}
		} moyenne2 /= nTaille/4;

		for(int i = 0; i < nH/2; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			variance2 += pow(ImgIn[i*nW+j] - moyenne2, 2);
   			ImgOut[i*nW+j] = moyenne2;
	  	}
		}
		variance2 /= nTaille/4;


		//bas gauche
		for(int i = nH/2; i < nH; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			moyenne3 += ImgIn[i*nW+j];
	  	}
		} moyenne3 /= nTaille/4;

		for(int i = nH/2; i < nH; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			variance3 += pow(ImgIn[i*nW+j] - moyenne3, 2);
   			ImgOut[i*nW+j] = moyenne3;
	  	}
		}
		variance3 /= nTaille/4;


		//bas droit
		for(int i = nH/2; i < nH; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			moyenne4 += ImgIn[i*nW+j];
	  	}
		} moyenne4 /= nTaille/4;

		for(int i = nH/2; i < nH; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			variance4 += pow(ImgIn[i*nW+j] - moyenne4, 2);
   			ImgOut[i*nW+j] = moyenne4;
	  	}
		}
		variance4 /= nTaille/4;

		cout << "variance1 : " << variance1 << endl; 
		cout << "variance2 : " << variance2 << endl; 
		cout << "variance3 : " << variance3 << endl; 
		cout << "variance4 : " << variance4 << endl; 

  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

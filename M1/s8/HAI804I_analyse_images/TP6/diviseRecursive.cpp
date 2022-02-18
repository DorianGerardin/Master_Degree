// test_couleur.cpp : Seuille une image en niveau de gris
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "image_ppm.h"
#include "file.h"

OCTET *ImgIn, *ImgOut;

void divise(OCTET *ImgIn, int nH, int nW, int seuil) {

	float moyenne1 = 0;
  float moyenne2 = 0;
  float moyenne3 = 0;
  float moyenne4 = 0;

  float variance1 = 0;
  float variance2 = 0;
  float variance3 = 0;
  float variance4 = 0;

  int nTaille = nH*nW;

  OCTET newImg1[nTaille/4];
  OCTET newImg2[nTaille/4];
  OCTET newImg3[nTaille/4];
  OCTET newImg4[nTaille/4];

	if(nH <= 8 || nW <= 8) {
		//nothing
	} else {
		int incr = 0;
		cout << "yo" << endl;  
		for(int i = 0; i < nH/2; i++) {
   		for(int j = 0; j < nW/2; j++) { 
   			incr++;
   			moyenne1 += ImgIn[i*nW+j];
   			//newImg1[incr] = ImgIn[i*nW+j]; 
   			cout << "indice1 : " << (int)incr << endl;  
   			cout << "taille1 : " << (int)nTaille/4 << endl;  
	  	}
		} moyenne1 /= nTaille/4; 

		for(int i = 0; i < nH/2; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			variance1 += pow(ImgIn[i*nW+j] - moyenne1, 2);
   			ImgOut[i*nW+j] = moyenne1;
	  	}
		}
		variance1 /= nTaille/4;
		cout << "variance1 : " << variance1 << endl;  
		if(variance1 > seuil) {
			divise(ImgIn, nH/2, nW/2, seuil);
		}

		incr = 0;
		//haut droit
		for(int i = 0; i < nH/2; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			incr++;
   			moyenne2 += ImgIn[i*nW+j];
   			///newImg2[incr] = ImgIn[i*nW+j];
   			cout << "indice2 : " << (int)incr << endl;  
   			cout << "taille2 : " << (int)nTaille/4 << endl; 
	  	}
		} moyenne2 /= nTaille/4;
		cout << "moyenne 2: " << moyenne2 << endl; 

		for(int i = 0; i < nH/2; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			variance2 += pow(ImgIn[i*nW+j] - moyenne2, 2);
   			ImgOut[i*nW+j] = moyenne2;
	  	}
		}
		variance2 /= nTaille/4;
		cout << "variance2 : " << variance2 << endl; 
		if(variance2 > seuil) {
			divise(ImgIn, nH/2, nW/2, seuil);
		}

		incr = 0;
		//bas gauche
		for(int i = nH/2; i < nH; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			incr++;
   			moyenne3 += ImgIn[i*nW+j];
   			//newImg3[incr] = ImgIn[i*nW+j];
	  	}
		} moyenne3 /= nTaille/4;

		for(int i = nH/2; i < nH; i++) {
   		for(int j = 0; j < nW/2; j++) {
   			variance3 += pow(ImgIn[i*nW+j] - moyenne3, 2);
   			ImgOut[i*nW+j] = moyenne3;
	  	}
		}
		variance3 /= nTaille/4; 
		cout << "variance3 : " << variance3 << endl;  
		if(variance3 > seuil) {
			divise(ImgIn, nH/2, nW/2, seuil);
		}

		incr = 0;
		//bas droit
		for(int i = nH/2; i < nH; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			incr++;
   			moyenne4 += ImgIn[i*nW+j];
   			//newImg4[incr] = ImgIn[i*nW+j]; 
	  	}
		} moyenne4 /= nTaille/4;

		for(int i = nH/2; i < nH; i++) {
   		for(int j = nW/2; j < nW; j++) {
   			variance4 += pow(ImgIn[i*nW+j] - moyenne4, 2);
   			ImgOut[i*nW+j] = moyenne4;
	  	}
		}
		variance4 /= nTaille/4;
		cout << "variance4 : " << variance4 << endl; 
		if(variance4 > seuil) {
			divise(ImgIn, nH/2, nW/2, seuil);
		}
	}

}

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
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   divise(ImgIn, nH, nW, 5);
  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

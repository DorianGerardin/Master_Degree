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

   for(int i = 0; i < nH; i++) {
   		for(int j = 0; j < nH; j++) {
	    		//voisin haut gauche
	        if(i-1 >= 0 && j-1 >= 0) {
	        	int value;
	        	if(ImgIn[(i-1)*nW+(j-1)] + 128 > 255) value = 255;
	        	else if(ImgIn[(i-1)*nW+(j-1)] + 128 < 0) value = 0;
	        	else value = ImgIn[(i-1)*nW+(j-1)] + 128;
	        	ImgOut[(i-1)*nW+(j-1)] = value;
	        }
	         //voisin haut
	        if(i-1 >= 0) {
	          int value;
	          if(ImgIn[(i-1)*nW+j] -1 + 128 > 255) value = 255;
	        	else if(ImgIn[(i-1)*nW+j] -1 + 128 < 0) value = 0;
	        	else value = ImgIn[(i-1)*nW+j] -1 + 128;
	        	ImgOut[(i-1)*nW+(j-1)] = value;
	        }
	         //voisin haut droit
	        if(i-1 >= 0 && j+1< nW) {
	          int value;
	          if(ImgIn[(i-1)*nW+(j+1)] + 128 > 255) value = 255;
	        	else if(ImgIn[(i-1)*nW+(j+1)] + 128 < 0) value = 0;
	        	else value = ImgIn[(i-1)*nW+(j+1)] + 128;
	        	ImgOut[(i-1)*nW+(j+1)] = value;
	        }
	         //voisin droit
	        if(j+1 < nW) {
	          int value;
	          if(ImgIn[i*nW+(j+1)] -1 + 128 > 255) value = 255;
	        	else if(ImgIn[i*nW+(j+1)] -1 + 128 < 0) value = 0;
	        	else value = ImgIn[i*nW+(j+1)] -1 + 128;
	        	ImgOut[i*nW+(j+1)] = value;
	        }
	        //voisin bas droit
	        if(i+1 < nH && j+1 < nW) {
	          int value;
	          if(ImgIn[(i+1)*nW+(j+1)] + 128 > 255) value = 255;
	        	else if(ImgIn[(i+1)*nW+(j+1)] + 128 < 0) value = 0;
	        	else value = ImgIn[(i+1)*nW+(j+1)] + 128;
	        	ImgOut[(i+1)*nW+(j+1)] = value;
	        }
	        //voisin bas
	        if(i+1 < nH) {
	          int value;
	          if(ImgIn[(i+1)*nW+j] -1 + 128 > 255) value = 255;
	        	else if(ImgIn[(i+1)*nW+j] -1 + 128 < 0) value = 0;
	        	else value = ImgIn[(i+1)*nW+j] -1 + 128;
	        	ImgOut[(i+1)*nW+j] = value;
	        }
	        //voisin bas gauche
	        if(i+1 < nH && j-1 >= 0) {
	          int value;
	          if(ImgIn[(i+1)*nW+(j-1)] + 128 > 255) value = 255;
	        	else if(ImgIn[(i+1)*nW+(j-1)] + 128 < 0) value = 0;
	        	else value = ImgIn[(i+1)*nW+(j-1)] + 128;
	        	ImgOut[(i+1)*nW+(j-1)] = value;
	        }
	          //voisin gauche
	        if(j-1 >= 0) {
	          int value;
	          if(ImgIn[i*nW+(j-1)] -1 + 128 > 255) value = 255;
	        	else if(ImgIn[i*nW+(j-1)] -1 + 128 < 0) value = 0;
	        	else value = ImgIn[i*nW+(j-1)] -1 + 128;
	        	ImgOut[i*nW+(j-1)] = value;
	  			}
	  			int value;
          if(ImgIn[i*nW+(j-1)] + 4 + 128 > 255) value = 255;
        	else if(ImgIn[i*nW+(j-1)] + 4 + 128 < 0) value = 0;
        	else value = ImgIn[i*nW+(j-1)] + 4 + 128;
        	ImgOut[i*nW+j] = value;
	  	}
	}


  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

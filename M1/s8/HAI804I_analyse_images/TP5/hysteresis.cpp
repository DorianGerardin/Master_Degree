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
   int nH, nW, nTaille, nTaille3, SH, SB;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn ImageOut SH SB\n"); 
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
   sscanf (argv[3],"%d",&SH);
   sscanf (argv[4],"%d",&SB);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   for(int i = 0; i < nH; i++) {
   		for(int j = 0; j < nH; j++) {
    		if(ImgIn[i*nW+j] > SH) ImgIn[i*nW+j] = 255;
    		else if(ImgIn[i*nW+j] < SH && ImgIn[i*nW+j] > SB) {
	    		//voisin haut gauche
		        if(i-1 >= 0 && j-1 >= 0) {
		            if(ImgIn[(i-1)*nW+(j-1)] > SH) ImgOut[i*nW+j] = 255;
		        }
		         //voisin haut
		        if(i-1 >= 0) {
		            if(ImgIn[(i-1)*nW+j] > SH) ImgOut[i*nW+j] = 255;
		        }
		         //voisin haut droit
		        if(i-1 >= 0 && j+1<= nW) {
		            if(ImgIn[(i-1)*nW+(j+1)] > SH) ImgOut[i*nW+j] = 255;
		        }
		         //voisin droit
		        if(j+1 <= nW) {
		            if(ImgIn[i*nW+(j+1)] > SH) ImgOut[i*nW+j] = 255;
		        }
		        //voisin bas droit
		        if(i+1 <= nH && j+1 <= nW) {
		            if(ImgIn[(i+1)*nW+(j+1)] > SH) ImgOut[i*nW+j] = 255;
		        }
		        //voisin bas
		        if(i+1 <= nH) {
		            if(ImgIn[(i+1)*nW+j] > SH) ImgOut[i*nW+j] = 255;
		        }
		        //voisin bas gauche
		        if(i+1 <= nH && j-1 >= 0) {
		            if(ImgIn[(i+1)*nW+(j-1)] > SH) ImgOut[i*nW+j] = 255;
		        }
		          //voisin gauche
		        if(j-1 >= 0) {
		            if(ImgIn[i*nW+(j-1)] > SH) ImgOut[i*nW+j] = 255;
		    	}
		    	else ImgOut[i*nW+j] = 0;
	  		}
	  		else ImgOut[i*nW+j] = 0;
	  	}
	}


  
   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

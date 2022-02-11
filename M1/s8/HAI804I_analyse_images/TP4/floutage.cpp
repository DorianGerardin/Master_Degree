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
       printf("Usage: ImageIn.pgm ImageOut \n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".ppm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  string s2 = string(folder) + string(argv[2]) + string(extension);
  char* filename1 = stringToCharArray(s1);
  char* filename2 = stringToCharArray(s2);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (filename2,"%s",cNomImgEcrite);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
   nTaille3 = nTaille * 3;
  
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

   int R[nTaille];
   int G[nTaille];
   int B[nTaille];
 	
   int nH3 = nH*3;
   int nW3 = nW*3;
   for (int i = 0; i < nTaille3; i+=3)
   {
	   	R[i] = ImgIn[i];
	   	G[i] = ImgIn[i+1];
	   	B[i] = ImgIn[i+2];
   }

   for (int i = 0; i < nH; i+=3)
   {
   		for (int j = 0; j < nW; j+=3)
   		{
   			//1er pixel
   			if(i == 0 && j == 0) {
   				ImgOut[i*nW+j] = 1/3 * (R[i*nW+j+1] + R[(i+1)*nW+j] + R[(i+1)*nW+j+1]);
   				ImgOut[i*nW+j+1] = 1/3 * (G[i*nW+j+1] + G[(i+1)*nW+j] + G[(i+1)*nW+j+1]);
   				ImgOut[i*nW+j+2] = 1/3 * (B[i*nW+j+1] + B[(i+1)*nW+j] + B[(i+1)*nW+j+1]);
   			}
   			//pixel haut droit
   			else if(j == nW-4 && i == 0) {
   				ImgOut[i*nW+j] = 1/3 * (R[(i+1)*nW+j] + R[(i+1)*nW+j-1] + R[i*nW+j-1]);
   				ImgOut[i*nW+j+1] = 1/3 * (G[(i+1)*nW+j] + G[(i+1)*nW+j-1] + G[i*nW+j-1]);
   				ImgOut[i*nW+j+2] = 1/3 * (B[(i+1)*nW+j] + B[(i+1)*nW+j-1] + B[i*nW+j-1]);
   			}
   			//pixel bas gauche
   			else if(j == 0 && i == nH-1) {
   				ImgOut[i*nW+j] = 1/3 * (R[(i-1)*nW+j] + R[(i-1)*nW+j-1] + R[i*nW+j-1]);
   				ImgOut[i*nW+j+1] = 1/3 * (G[(i-1)*nW+j] + G[(i-1)*nW+j-1] + G[i*nW+j-1]);
   				ImgOut[i*nW+j+2] = 1/3 * (B[(i-1)*nW+j] + B[(i-1)*nW+j-1] + B[i*nW+j-1]);
   			}
   			//pixel bas droite
   			else if(j == nW-3 && i == nH-1) {
   				ImgOut[i*nW+j] = 1/3 * (R[(i-1)*nW+j] + R[(i-1)*nW+j+1] + R[i*nW+j+1]);
   				ImgOut[i*nW+j+1] = 1/3 * (G[(i-1)*nW+j] + G[(i-1)*nW+j+1] + G[i*nW+j+1]);
   				ImgOut[i*nW+j+2] = 1/3 * (B[(i-1)*nW+j] + B[(i-1)*nW+j+1] + B[i*nW+j+1]);
   			}

   			//1ere ligne
   			else if(i == 0 && j > 0) {
   				ImgOut[i*nW+j] = 1/6 * (
   					R[i*nW+j-1] + R[i*nW+j] + R[i*nW+j+1] 
   					+ R[(i+1)*nW+j-1] + R[(i+1)*nW+j] + R[(i+1)*nW+j+1]);

   				//Vert
   				ImgOut[i*nW+j+1] = 1/6 * (
   					G[i*nW+j-1] + G[i*nW+j] + G[i*nW+j+1] 
   					+ G[(i+1)*nW+j-1] + G[(i+1)*nW+j] + G[(i+1)*nW+j+1]);

   				//Bleu
   				ImgOut[i*nW+j+2] = 1/6 * (
   					B[i*nW+j-1] + B[i*nW+j] + B[i*nW+j+1] 
   					+ B[(i+1)*nW+j-1] + B[(i+1)*nW+j] + B[(i+1)*nW+j+1]);
   			}

   			//1ere colonne
   			else if(j == 0 && i > 0) {
   				//Rouge
   				ImgOut[i*nW+j] = 1/6 * (
   					R[(i-1)*nW+j] + R[(i-1)*nW+j+1]
   					+ R[i*nW+j] + R[i*nW+j+1] 
   					+ R[(i+1)*nW+j] + R[(i+1)*nW+j+1]);

   				//Vert
   				ImgOut[i*nW+j+1] = 1/6 * (
   					G[(i-1)*nW+j] + G[(i-1)*nW+j+1]
   					+ G[i*nW+j] + G[i*nW+j+1] 
   					+ G[(i+1)*nW+j] + G[(i+1)*nW+j+1]);

   				//Bleu
   				ImgOut[i*nW+j+2] = 1/6 * (
   					B[(i-1)*nW+j] + B[(i-1)*nW+j+1]
   					+ B[i*nW+j] + B[i*nW+j+1] 
   					+ B[(i+1)*nW+j] + B[(i+1)*nW+j+1]);
   			}

   			else {
   				//Rouge
   				ImgOut[i*nW+j] = 1/9 * (
   					R[(i-1)*nW+j-1] + R[(i-1)*nW+j] + R[(i-1)*nW+j+1]
   					+ R[i*nW+j-1] + R[i*nW+j] + R[i*nW+j+1] 
   					+ R[(i+1)*nW+j-1] + R[(i+1)*nW+j] + R[(i+1)*nW+j+1]);

   				//Vert
   				ImgOut[i*nW+j+1] = 1/9 * (
   					G[(i-1)*nW+j-1] + G[(i-1)*nW+j] + G[(i-1)*nW+j+1]
   					+ G[i*nW+j-1] + G[i*nW+j] + G[i*nW+j+1] 
   					+ G[(i+1)*nW+j-1] + G[(i+1)*nW+j] + G[(i+1)*nW+j+1]);

   				//Bleu
   				ImgOut[i*nW+j+2] = 1/9 * (
   					B[(i-1)*nW+j-1] + B[(i-1)*nW+j] + B[(i-1)*nW+j+1]
   					+ B[i*nW+j-1] + B[i*nW+j] + B[i*nW+j+1] 
   					+ B[(i+1)*nW+j-1] + B[(i+1)*nW+j] + B[(i+1)*nW+j+1]);	
   			}

   		}
   }

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

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
   int nH, nW, nTaille, nTaille3, Smin, Smax;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn ImageOut SeuilMin SeuilMax\n"); 
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
   sscanf (argv[3],"%d",&Smin);
   sscanf (argv[4],"%d",&Smax);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
   nTaille3 = nH * nW * 3;
  
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);

   for(int i = 0; i < nTaille3; i+=3) {
    	
		if(ImgIn[i] < Smin) ImgOut[i] = Smin;
		else if(ImgIn[i] > Smax) ImgOut[i] = Smax;
		else ImgOut[i] = ImgIn[i];

		if(ImgIn[i+1] < Smin) ImgOut[i+1] = Smin;
		else if(ImgIn[i+1] > Smax) ImgOut[i+1] = Smax;
		else ImgOut[i+1] = ImgIn[i+1];

		if(ImgIn[i+2] < Smin) ImgOut[i+2] = Smin;
		else if(ImgIn[i+2] > Smax) ImgOut[i+2] = Smax;
		else ImgOut[i+2] = ImgIn[i+2];
		
	}

   ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn);
   return 1;
}

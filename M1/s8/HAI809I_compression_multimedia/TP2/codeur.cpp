#include "ImageBase.h"
#include "ImageBase.cpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h> 
#include <cmath>
#include <cmath>
using namespace std;

struct pixel {
	int r,g,b;
};

int main(int argc, char **argv)
{

	srand(time(NULL));
	///////////////////////////////////////// Exemple d'un seuillage d'image
	char cNomImgLue[250], cNomImgEcriteR[250], cNomImgEcriteG[250], cNomImgEcriteB[250];
	
  
	if (argc != 5) 
	{
		printf("Usage: ImageIn.ppm ImageOut1.pgm ImageOut2.pgm ImageOut3.pgm\n"); 
		return 1;
	}
	sscanf (argv[1],"%s",cNomImgLue);
	sscanf (argv[2],"%s",cNomImgEcriteR);
	sscanf (argv[3],"%s",cNomImgEcriteG);
	sscanf (argv[4],"%s",cNomImgEcriteB);
	
	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOutR(imIn.getWidth(), imIn.getHeight(), false);
	ImageBase imOutG(imIn.getWidth()/2, imIn.getHeight()/2, false);
	ImageBase imOutB(imIn.getWidth()/2, imIn.getHeight()/2, false);

	for(int y = 0; y < imIn.getHeight(); ++y) {
		for(int x = 0; x < imIn.getWidth(); ++x)
		{
			//image R
			imOutR[y][x] = imIn[y*3][x*3+0];
		}
	}

	for(int y = 0; y < imIn.getHeight()/2; ++y) {
		for(int x = 0; x < imIn.getWidth()/2; ++x)
		{
			//image G reduite
			imOutG[y][x] = imIn[y*6][x*6+1];
			//image B reduite
			imOutB[y][x] = imIn[y*6][x*6+2];
		}
	}
	

	imOutR.save(cNomImgEcriteR);
	imOutG.save(cNomImgEcriteG);
	imOutB.save(cNomImgEcriteB);
		

	
	
	///////////////////////////////////////// Exemple de création d'une image couleur
	/*ImageBase imC(50, 100, true);

	for(int y = 0; y < imC.getHeight(); ++y)
		for(int x = 0; x < imC.getWidth(); ++x)
		{
			imC[y*3][x*3+0] = 200; // R
			imC[y*3][x*3+1] = 0; // G
			imC[y*3][x*3+2] = 0; // B
		}
		
	imC.save("imC.ppm");*/
		



	///////////////////////////////////////// Exemple de création d'une image en niveau de gris
	/*ImageBase imG(50, 100, false);

	for(int y = 0; y < imG.getHeight(); ++y)
		for(int x = 0; x < imG.getWidth(); ++x)
			imG[y][x] = 50;

	imG.save("imG.pgm");*/




	//ImageBase imC2, imG2;
	
	///////////////////////////////////////// Exemple lecture image couleur
	//imC2.load("imC.ppm");
	///////////////////////////////////////// Exemple lecture image en niveau de gris
	//imG2.load("imG.pgm");
	
	

	///////////////////////////////////////// Exemple de récupération d'un plan de l'image
	/*ImageBase *R = imC2.getPlan(ImageBase::PLAN_R);
	R->save("R.pgm");
	delete R;*/
	


	return 0;
}
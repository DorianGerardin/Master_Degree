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
	char cNomImgLueR[250], cNomImgLueG[250], cNomImgLueB[250], cNomImgEcrite[250];
	
  
	if (argc != 5) 
	{
		printf("Usage: ImageInR.ppm ImageInG.pgm ImageInB.pgm ImageOut.pgm\n"); 
		return 1;
	}
	sscanf (argv[1],"%s",cNomImgLueR);
	sscanf (argv[2],"%s",cNomImgLueG);
	sscanf (argv[3],"%s",cNomImgLueB);
	sscanf (argv[4],"%s",cNomImgEcrite);
	
	//ImageBase imIn, imOut;
	ImageBase imInR, imInG, imInB;
	imInR.load(cNomImgLueR);
	imInG.load(cNomImgLueG);
	imInB.load(cNomImgLueB);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOut(imInR.getWidth(), imInR.getHeight(), imInR.getColor());

	for(int y = 0; y < imInR.getHeight(); y+=2) {
		for(int x = 0; x < imInR.getWidth(); x+=2)
		{
			imOut[y*3][x*3] = imInR[y*3][x*3+0];

			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					imOut[y*3+i][x*3+j+1] = imInG[y/2][x/2];
					imOut[y*3+i][x*3+j+2] = imInB[y/2][x/2];
				}
			}
		}
	}
	
	imOut.save(cNomImgEcrite);
	
	return 0;
}
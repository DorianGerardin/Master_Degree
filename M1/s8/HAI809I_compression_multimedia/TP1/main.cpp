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

float distance(pixel &p, pixel &c) {
	float distance = sqrt((pow(p.r-c.r, 2))+(pow(p.g-c.g, 2))+(pow(p.b-c.b, 2)));
	return distance;
}


int main(int argc, char **argv)
{

	srand(time(NULL));
	///////////////////////////////////////// Exemple d'un seuillage d'image
	char cNomImgLue[250], cNomImgEcrite[250];
	int nbIteration;
  
	if (argc != 4) 
	{
		printf("Usage: ImageIn.pgm ImageOut.pgm nbIteration\n"); 
		return 1;
	}
	sscanf (argv[1],"%s",cNomImgLue);
	sscanf (argv[2],"%s",cNomImgEcrite);
	sscanf (argv[3],"%d",&nbIteration);
	
	//ImageBase imIn, imOut;
	ImageBase imIn;
	imIn.load(cNomImgLue);

	//ImageBase imG(imIn.getWidth(), imIn.getHeight(), imIn.getColor());
	ImageBase imOut(imIn.getWidth(), imIn.getHeight(), imIn.getColor());

	int classes[imIn.getWidth()*imIn.getHeight()];

	int randomHeight = rand() % imIn.getHeight(); 
	int randomWidth = rand() % imIn.getWidth();
	//int c1 = imIn[randomHeight][randomWidth]; 

	int randomHeight2 = rand() % imIn.getHeight(); 
	int randomWidth2 = rand() % imIn.getWidth();
	//int c2 = imIn[randomHeight2][randomWidth2]; 

	int moyenneCR = 0;
	int moyenneCG = 0;
	int moyenneCB = 0;

	int moyenneC2R = 0;
	int moyenneC2G = 0;
	int moyenneC2B = 0;

	int countC = 1;
	int countC2 = 1;

	int i = 0;

	for(int y = 0; y < imIn.getHeight(); ++y) {
		for(int x = 0; x < imIn.getWidth(); ++x)
		{
			pixel p,c,c2;
			p.r = imIn[y*3][x*3+0];
			p.g = imIn[y*3][x*3+1];
			p.b = imIn[y*3][x*3+2];
		
			c.r = imIn[randomHeight*3][randomWidth*3+0];
			c.g = imIn[randomHeight*3][randomWidth*3+1];
			c.b = imIn[randomHeight*3][randomWidth*3+2];

			c2.r = imIn[randomHeight2*3][randomWidth2*3+0];
			c2.g = imIn[randomHeight2*3][randomWidth2*3+1];
			c2.b = imIn[randomHeight2*3][randomWidth2*3+2];

			float d1 = distance(p, c);
			float d2 = distance(p, c2);

			if(d1 < d2) {
				// Class 1
				moyenneCR += c.r;
				moyenneCG += c.g;
				moyenneCB += c.b;
				classes[i] = 0;
				countC++;
			} else {
				//Class 2
				moyenneC2R += c2.r;
				moyenneC2G += c2.g;
				moyenneC2B += c2.b;
				classes[i] = 1;
				countC2++;
			}
			i++;
		}
	}
	moyenneCR /= countC;
	moyenneCG /= countC;
	moyenneCB /= countC;

	moyenneC2R /= countC2;
	moyenneC2G /= countC2;
	moyenneC2B /= countC2;

	for (int j = 0; j < nbIteration-1; ++j)
	{
		i = 0;
		countC = 1;
		countC2 = 1;
		for(int y = 0; y < imIn.getHeight(); ++y) {
			for(int x = 0; x < imIn.getWidth(); ++x)
			{
				pixel p,c,c2;
				p.r = imIn[y*3][x*3+0];
				p.g = imIn[y*3][x*3+1];
				p.b = imIn[y*3][x*3+2];
			
				c.r = moyenneCR;
				c.g = moyenneCG;
				c.b = moyenneCB;

				c2.g = moyenneC2G;
				c2.r = moyenneC2R;
				c2.b = moyenneC2B;

				float d1 = distance(p, c);
				float d2 = distance(p, c2);

				if(d1 < d2) {
					// Class 1
					moyenneCR += c.r;
					moyenneCG += c.g;
					moyenneCB += c.b;
					classes[i] = 0;
					countC++;
				} else {
					//Class 2
					moyenneC2R += c2.r;
					moyenneC2G += c2.g;
					moyenneC2B += c2.b;
					classes[i] = 1;
					countC2++;
				}
				i++;
			}
		}
		cout << "c : " << countC << endl;
		cout << "c2 : " << countC2 << endl;
		moyenneCR /= countC;
		moyenneCG /= countC;
		moyenneCB /= countC;

		moyenneC2R /= countC2;
		moyenneC2G /= countC2;
		moyenneC2B /= countC2;
	}

	i = 0;
	for(int y = 0; y < imIn.getHeight(); ++y) {
		for(int x = 0; x < imIn.getWidth(); ++x)
		{
			if(classes[i] == 0) {
				imOut[y*3][x*3+0] = moyenneCR;
				imOut[y*3][x*3+1] = moyenneCG;
				imOut[y*3][x*3+2] = moyenneCB;
			} else if(classes[i] == 1) {
				imOut[y*3][x*3+0] = moyenneC2R;
				imOut[y*3][x*3+1] = moyenneC2G;
				imOut[y*3][x*3+2] = moyenneC2B;
			}
			i++;
		}
	}



	imOut.save(cNomImgEcrite);
		

	
	
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
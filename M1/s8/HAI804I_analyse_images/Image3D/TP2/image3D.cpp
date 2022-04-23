#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned short U_SHORT;
typedef unsigned int U_INT;

struct Image3D {

    U_INT dimX, dimY, dimZ, sizeFile;
    float sizeXVoxel, sizeYVoxel, sizeZVoxel;
    U_SHORT *data;

    U_SHORT getValue(U_INT i, U_INT j, U_INT k) {
   	 return data[(k * (dimX * dimY) + (dimY - 1 - j) * dimX + i)];
    }

    U_SHORT getMin() {
   	 U_SHORT min = data[0];
   	 for (int i = 0; i < sizeFile; ++i)
   	 {
   		 U_SHORT value = data[i];
   		 if(value < min) min = value;
   	 }
   	 return min;
    }

    U_SHORT getMax() {
   	 U_SHORT max = data[0];
   	 for (int i = 0; i < sizeFile; ++i)
   	 {
   		 U_SHORT value = data[i];
   		 if(value > max) max = value;
   	 }
   	 return max;
    }

    void intoMesh(U_SHORT treshold, string filename) {

    string const nomFichier(filename);
  	 ofstream flux(nomFichier.c_str());

	if(flux) {

   	 flux << "solid name" << endl;
    
   	 for (U_INT i = 1; i < dimX - 1; i++)
   	 {
   		 for (U_INT j = 1; j < dimY - 1; j++)
   		 {
   			 for (U_INT k = 1; k < dimZ - 1; k++)
   			 {
   				 vector<float> s0, s1, s2, s3, s4, s5, s6, s7;

   				 s0.push_back((i-0.5)*sizeXVoxel);
   				 s0.push_back((j-0.5)*sizeYVoxel);
   				 s0.push_back((k-0.5)*sizeZVoxel);

   				 s1.push_back((i+0.5)*sizeXVoxel);
   				 s1.push_back((j-0.5)*sizeYVoxel);
   				 s1.push_back((k-0.5)*sizeZVoxel);

   				 s2.push_back((i+0.5)*sizeXVoxel);
   				 s2.push_back((j+0.5)*sizeYVoxel);
   				 s2.push_back((k-0.5)*sizeZVoxel);

   				 s3.push_back((i-0.5)*sizeXVoxel);
   				 s3.push_back((j+0.5)*sizeYVoxel);
   				 s3.push_back((k-0.5)*sizeZVoxel);

   				 s4.push_back((i-0.5)*sizeXVoxel);
   				 s4.push_back((j-0.5)*sizeYVoxel);
   				 s4.push_back((k+0.5)*sizeZVoxel);

   				 s5.push_back((i+0.5)*sizeXVoxel);
   				 s5.push_back((j-0.5)*sizeYVoxel);
   				 s5.push_back((k+0.5)*sizeZVoxel);

   				 s6.push_back((i+0.5)*sizeXVoxel);
   				 s6.push_back((j+0.5)*sizeYVoxel);
   				 s6.push_back((k+0.5)*sizeZVoxel);

   				 s7.push_back((i-0.5)*sizeXVoxel);
   				 s7.push_back((j+0.5)*sizeYVoxel);
   				 s7.push_back((k+0.5)*sizeZVoxel);

   		if(getValue(i, j, k) >= treshold) {
   		//voxel gauche
   			if(getValue(i-1, j, k) < treshold) {
   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s3[0] << " " << s3[1] << " " << s3[2] << endl;
   		flux << "vertex " << s0[0] << " " << s0[1] << " " << s0[2] << endl;
   		flux << "vertex " << s4[0] << " " << s4[1] << " " << s4[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s4[0] << " " << s4[1] << " " << s4[2] << endl;
   		flux << "vertex " << s7[0] << " " << s7[1] << " " << s7[2] << endl;
   		flux << "vertex " << s3[0] << " " << s3[1] << " " << s3[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   	}

   		//voxel haut
   	if(getValue(i, j, k+1) < treshold) {
   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s4[0] << " " << s4[1] << " " << s4[2] << endl;
   		flux << "vertex " << s5[0] << " " << s5[1] << " " << s5[2] << endl;
   		flux << "vertex " << s6[0] << " " << s6[1] << " " << s6[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s6[0] << " " << s6[1] << " " << s6[2] << endl;
   		flux << "vertex " << s7[0] << " " << s7[1] << " " << s7[2] << endl;
   		flux << "vertex " << s4[0] << " " << s4[1] << " " << s4[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   	}

   	//voxel droit
   	if(getValue(i+1, j, k) < treshold) {
   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s1[0] << " " << s1[1] << " " << s1[2] << endl;
   		flux << "vertex " << s2[0] << " " << s2[1] << " " << s2[2] << endl;
   		flux << "vertex " << s6[0] << " " << s6[1] << " " << s6[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s6[0] << " " << s6[1] << " " << s6[2] << endl;
   		flux << "vertex " << s5[0] << " " << s5[1] << " " << s5[2] << endl;
   		flux << "vertex " << s1[0] << " " << s1[1] << " " << s1[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   	}

   					 // //voxel bas
   	if(getValue(i, j, k-1) < treshold) {
   	     flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s3[0] << " " << s3[1] << " " << s3[2] << endl;
   		flux << "vertex " << s2[0] << " " << s2[1] << " " << s2[2] << endl;
   		flux << "vertex " << s1[0] << " " << s1[1] << " " << s1[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s1[0] << " " << s1[1] << " " << s1[2] << endl;
   		flux << "vertex " << s0[0] << " " << s0[1] << " " << s0[2] << endl;
   		flux << "vertex " << s3[0] << " " << s3[1] << " " << s3[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   	}

   	//voxel devant
   	if(getValue(i, j-1, k) < treshold) {
   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s0[0] << " " << s0[1] << " " << s0[2] << endl;
   		flux << "vertex " << s1[0] << " " << s1[1] << " " << s1[2] << endl;
   		flux << "vertex " << s5[0] << " " << s5[1] << " " << s5[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s5[0] << " " << s5[1] << " " << s5[2] << endl;
   		flux << "vertex " << s4[0] << " " << s4[1] << " " << s4[2] << endl;
   		flux << "vertex " << s0[0] << " " << s0[1] << " " << s0[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   	}

   	//voxel derriere
   	if(getValue(i, j+1, k) < treshold) {
   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s2[0] << " " << s2[1] << " " << s2[2] << endl;
   		flux << "vertex " << s3[0] << " " << s3[1] << " " << s3[2] << endl;
   		flux << "vertex " << s7[0] << " " << s7[1] << " " << s7[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;

   		flux << "facet normal 0 0 0" << endl;
   		flux << "outer loop" << endl;
   		flux << "vertex " << s7[0] << " " << s7[1] << " " << s7[2] << endl;
   		flux << "vertex " << s6[0] << " " << s6[1] << " " << s6[2] << endl;
   		flux << "vertex " << s2[0] << " " << s2[1] << " " << s2[2] << endl;
   		flux << "endloop" << endl;
   		flux << "endfacet" << endl;
   					 }
   				 }
   			 }
   		 }
   	 }
   	 flux << "endsolid name" << endl;
    }
    else {
   	 cout << "ERREUR: Impossible d'ouvrir le fichier " << filename << endl;
   	 exit(EXIT_FAILURE);
	}
}

};

U_SHORT swich(U_SHORT s) {
	 
    U_SHORT hi = (s & 0xff00);
    U_SHORT lo = (s & 0xff);
	return (lo << 8) | (hi >> 8);
}

void readImg(char *nom_image, Image3D *img) {

    FILE *file;
    size_t result;

    file = fopen ( nom_image , "rb" );
    if (file==NULL) {
   	 printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
         exit(EXIT_FAILURE);
    }

    img->data = (U_SHORT*) malloc (sizeof(U_SHORT)*img->sizeFile);
    if (img->data == NULL) {
   	 printf("\nErreur de mémoire \n");
         exit(EXIT_FAILURE);
    }

    result = fread (img->data, sizeof(U_SHORT), img->sizeFile, file);
    for (int i = 0; i < img->sizeFile; ++i)
    {    
   	 U_SHORT value = img->data[i];
   	 img->data[i] = swich(value);
    }

      if (result != img->sizeFile) {
     	 printf("\nErreur de lecture \n");
         exit(EXIT_FAILURE);
      }

      fclose (file);
}

//-----------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

    char cNomImgLue[250], cNomImgEcrite[250];
    U_SHORT treshold;
    U_INT dimX, dimY, dimZ;
    float sizeXVoxel, sizeYVoxel, sizeZVoxel;

    if (argc != 10)
	{
   	printf("Usage: ImageIn dimX dimY dimZ sizeXVoxel sizeYVoxel sizeZVoxel resultFilename treshold\n");
   	exit (1) ;
	}

	sscanf (argv[1],"%s", &cNomImgLue);
	sscanf (argv[2],"%u", &dimX);
	sscanf (argv[3],"%u", &dimY);
	sscanf (argv[4],"%u", &dimZ);
	sscanf (argv[5],"%f", &sizeXVoxel);
	sscanf (argv[6],"%f", &sizeYVoxel);
	sscanf (argv[7],"%f", &sizeZVoxel);
	sscanf (argv[8],"%s", &cNomImgEcrite);
	sscanf (argv[9],"%hu", &treshold);
	
Image3D *img = new Image3D();
	{
   	 img->dimX = dimX;
   	 img->dimY = dimY;
   	 img->dimZ = dimZ;
   	 img->sizeFile = dimX * dimY * dimZ;
   	 img->sizeXVoxel = sizeXVoxel;
   	 img->sizeYVoxel = sizeYVoxel;
   	 img->sizeZVoxel = sizeZVoxel;
	};

	readImg(cNomImgLue, img);

	img->intoMesh(treshold, cNomImgEcrite);

	free(img->data);
}
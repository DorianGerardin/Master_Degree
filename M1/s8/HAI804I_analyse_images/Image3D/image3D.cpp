#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

typedef unsigned short U_SHORT;
typedef unsigned int U_INT;

struct Image3D {

	U_INT dimX, dimY, dimZ, sizeFile;
	U_SHORT *data;

	U_SHORT getValue(U_INT i, U_INT j, U_INT k) {
		return data[k * (dimX * dimY) + (dimX - 1 - j) * dimX + i];
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

};

U_SHORT swich(U_SHORT s) {
 	
	U_SHORT hi = (s & 0xff00);
	U_SHORT lo = (s & 0xff); 
    return (lo << 8) | (hi >> 8);
}

U_SHORT* mip(U_INT visuAxis, Image3D *img) {

	U_INT sizeFile;
	U_SHORT *outData;

	switch(visuAxis) {
		case 1:
			sizeFile = img->dimY * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimY; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT max = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(k, i, j);
						if(value > max) max = value;
					}
					outData[i*img->dimZ + j] = max;
				}
			}
			break;
		case 2:
			sizeFile = img->dimX * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT max = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimY; ++k)
					{	
						U_SHORT value = img->getValue(i, k, j);
						if(value > max) max = value;
					}
					outData[i*img->dimZ + j] = max;
				}
			}
			break;
		case 3:
			sizeFile = img->dimX * img->dimY;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT max = img->data[i*img->dimX+j];
					for (int k = 0; k < img->dimZ; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value > max) max = value;
					}
					outData[i*img->dimY + j] = max;
				}
			}
			break;
	}
	return outData;
}

U_SHORT* minIP(U_INT visuAxis, Image3D *img) {

	U_INT sizeFile;
	U_SHORT *outData;

	switch(visuAxis) {
		case 1:
			sizeFile = img->dimY * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimY; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT min = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(k, i, j);
						if(value < min) min = value;
					}
					outData[i*img->dimZ + j] = min;
				}
			}
			break;
		case 2:
			sizeFile = img->dimX * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT min = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimY; ++k)
					{	
						U_SHORT value = img->getValue(i, k, j);
						if(value < min) min = value;
					}
					outData[i*img->dimZ + j] = min;
				}
			}
			break;
		case 3:
			sizeFile = img->dimX * img->dimY;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT min = img->data[i*img->dimY+j];
					for (int k = 0; k < img->dimZ; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value < min) min = value;
					}
					outData[i*img->dimY + j] = min;
				}
			}
			break;
	}
	return outData;
}

U_SHORT* aip(U_INT visuAxis, Image3D *img) {

	U_INT sizeFile;
	U_SHORT *outData;

	switch(visuAxis) {
		case 1:
			sizeFile = img->dimY * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimY; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT moyenne = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						moyenne += img->getValue(k, i, j);
					}
					outData[i*img->dimZ + j] = moyenne / img->dimX;
				}
			}
			break;
		case 2:
			sizeFile = img->dimX * img->dimZ;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT moyenne = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimY; ++k)
					{	
						moyenne += img->getValue(i, k, j);
					}
					outData[i*img->dimZ + j] = moyenne / img->dimY;
				}
			}
			break;
		case 3:
			sizeFile = img->dimX * img->dimY;
			outData = (U_SHORT*) malloc (sizeof(U_SHORT) * sizeFile);
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT moyenne = img->data[i*img->dimY+j];
					for (int k = 0; k < img->dimZ; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
					}
					outData[i*img->dimY + j] = moyenne / img->dimZ;
				}
			}
			break;
	}
	std::cout << "fin aip" << std::endl;
	return outData;
}

//----------------------------------------------------LECTURE/ECRITURE----------------------------------------------------

void readImg(char *nom_image, Image3D *img) {

	FILE * file;
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

void writeImg(char *nom_image, Image3D *img, U_INT visuAxis, U_INT visuMode) {

	FILE * file;
	size_t result;

	file = fopen ( nom_image , "wb" );

	if (file==NULL) {
		printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
	 	exit(EXIT_FAILURE);
	}

	U_INT sizeFile;
	U_SHORT *dataOutput;

	fprintf(file,"P5\r"); 

	switch(visuAxis) {
		case 1:		                         
			fprintf(file,"%d %d\r255\r", img->dimZ, img->dimY) ;		
			sizeFile = img->dimY * img->dimZ;
			break;
		case 2:                           
			fprintf(file,"%d %d\r255\r", img->dimX, img->dimZ);	
			sizeFile = img->dimX * img->dimZ;
			break;
		case 3:                             
			fprintf(file,"%d %d\r255\r", img->dimX, img->dimY);	
			sizeFile = img->dimX * img->dimY;
			break;
	}

	if(visuMode == 1) dataOutput = mip(visuAxis, img);
	else if(visuMode == 2) dataOutput = aip(visuAxis, img);
	else if(visuMode == 3) dataOutput = minIP(visuAxis, img);

	U_SHORT max = 0;
	for (int i = 0; i < sizeFile; ++i)
	{
		U_SHORT value = dataOutput[i];
		if(max < value) max = value;
	}
	for (int i = 0; i < sizeFile; ++i)
	{
		dataOutput[i] = floor(((float)dataOutput[i] / (float)max) * 255);
		std::cout << "value : " << dataOutput[i] << std::endl;
	}

	result = fwrite(dataOutput, sizeof(U_SHORT), sizeFile, file);

  	if (result != sizeFile) {
  		printf("\nErreur d'ecriture \n");
	 	exit(EXIT_FAILURE);
  	}

  	fclose (file);
}


//------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

	char cNomImgLue[250], cNomImgEcrite[250];
	U_INT visuAxis, visuMode;
	Image3D *img;

	if (argc != 8) 
    {
       printf("Usage: ImageIn dimX dimY dimZ ImageOut visuAxis visuMode\n"); 
       exit (1) ;
    }

    sscanf (argv[1],"%s", cNomImgLue);
    sscanf (argv[2],"%u", &img->dimX);
    sscanf (argv[3],"%u", &img->dimY);
    sscanf (argv[4],"%u", &img->dimZ);
    sscanf (argv[6],"%u", &visuAxis);
    sscanf (argv[7],"%u", &visuMode);

    img->sizeFile = img->dimX * img->dimY * img->dimZ;

    readImg(cNomImgLue, img);
    writeImg(argv[5], img, visuAxis, visuMode);

    U_SHORT min = img->getMin();
    U_SHORT max = img->getMax();

    free(img->data);

}
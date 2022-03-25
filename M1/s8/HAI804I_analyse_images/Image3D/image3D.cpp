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
		return data[i*dimX + j*dimY + k*dimZ];
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

			std::cout << "mip axe x" << std::endl;
			sizeFile = img->dimY * img->dimZ;
			for (int i = 0; i < img->dimZ; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT max = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value > max) max = value;
					}
					img->data[i*img->dimZ + j] = max;
				}
			}
			std::cout << "fin case 1" << std::endl;
			break;
		case 2:
			std::cout << "mip axe y" << std::endl;
			sizeFile = img->dimX * img->dimZ;
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT max = img->data[i*img->dimX+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value > max) max = value;
					}
					img->data[i*img->dimX + j] = max;
				}
			}
			break;
		case 3:
			std::cout << "mip axe z" << std::endl;
			sizeFile = img->dimX * img->dimY;
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT max = img->data[i*img->dimX+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value > max) max = value;
					}
					img->data[i*img->dimX + j] = max;
				}
			}
			break;
	}
	std::cout << "fin mip" << std::endl;
	return outData;
}

U_SHORT* minIP(U_INT visuAxis, Image3D *img) {

	U_INT sizeFile;
	U_SHORT *outData;

	switch(visuAxis) {
		case 1:

			//std::cout << "mip axe x" << std::endl;
			sizeFile = img->dimY * img->dimZ;
			//std::cout << "apres sizeFile" << std::endl;
			for (int i = 0; i < img->dimZ; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT min = img->data[i*img->dimZ+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value < min) min = value;
					}
					img->data[i*img->dimZ + j] = min;
				}
			}
			break;
		case 2:
			sizeFile = img->dimX * img->dimZ;
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimZ; ++j)
				{
					U_INT min = img->data[i*img->dimX+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value < min) min = value;
					}
					img->data[i*img->dimX + j] = min;
				}
			}
			break;
		case 3:
			sizeFile = img->dimX * img->dimY;
			for (int i = 0; i < img->dimX; ++i)
			{	
				for (int j = 0; j < img->dimY; ++j)
				{
					U_INT min = img->data[i*img->dimX+j];
					for (int k = 0; k < img->dimX; ++k)
					{	
						U_SHORT value = img->getValue(i, j, k);
						if(value < min) min = value;
					}
					img->data[i*img->dimX + j] = min;
				}
			}
			break;
	}
	std::cout << "fin mip" << std::endl;
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

	// dataOutput = (U_SHORT*) malloc (sizeof(U_SHORT)*sizeFile);
	// if (dataOutput == NULL) {
	// 	printf("\nErreur de mémoire \n");
	//  	exit(EXIT_FAILURE);
	// }

	switch(visuAxis) {
		case 1:
			sizeFile = img->dimY * img->dimZ;
			if(visuMode == 1) dataOutput = mip(visuAxis, img);
			std::cout << "get data after mip" << std::endl;
			break;
		case 2:
			sizeFile = img->dimX * img->dimZ;
			break;
		case 3:
			sizeFile = img->dimX * img->dimY;
			break;
	}

	result = fwrite(dataOutput, sizeof(U_SHORT), sizeFile, file);
	std::cout << "after write" << std::endl;
	// for (int i = 0; i < sizeFile; ++i)
	// {	std::cout << i << std::endl;
	// 	U_SHORT value = dataOutput[i];
	// 	dataOutput[i] = swich(value);
	// 	std::cout << "switch" << std::endl;
	// } std::cout << "after switch octet" << std::endl;

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

    std::cout << "debut scan" << std::endl;

    sscanf (argv[1],"%s", cNomImgLue);
    std::cout << "1" << std::endl;
    //sscanf (argv[5],"%s", cNomImgEcrite);
    //std::cout << cNomImgEcrite << std::endl;
    sscanf (argv[2],"%u", &img->dimX);

    std::cout << "3" << std::endl;
    sscanf (argv[3],"%u", &img->dimY);
    std::cout << "4" << std::endl;
    sscanf (argv[4],"%u", &img->dimZ);
    std::cout << "avant visuAxis" << std::endl;
    sscanf (argv[6],"%u", &visuAxis);
    sscanf (argv[7],"%u", &visuMode);

    std::cout << "fin scan" << std::endl;

    img->sizeFile = img->dimX * img->dimY * img->dimZ;

    readImg(cNomImgLue, img);
    writeImg(argv[5], img, visuAxis, visuMode);

    U_SHORT min = img->getMin();
    U_SHORT max = img->getMax();

    std::cout << "fin main" << std::endl;

    free(img->data);

}
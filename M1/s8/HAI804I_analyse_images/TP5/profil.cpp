#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, num;
  
  if (argc != 3) 
     {
       printf("Usage: ImageIn.pgm num\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".pgm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  char* filename1 = stringToCharArray(s1);
   
   sscanf (filename1,"%s",cNomImgLue) ;
   sscanf (argv[2],"%d",&num);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

   for(int x = 0; x < nW; ++x) {
      cout << x << " " << int(ImgIn[num*nW+x]) << endl;
    }

   free(ImgIn);
   return 1;
}

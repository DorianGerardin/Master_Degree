#include "image_ppm.h"
#include "file.h"

int main(int argc, char* argv[])
{
  int histoR[256] = {0};
  int histoG[256] = {0};
  int histoB[256] = {0};
  char cNomImgLue[250], cNomImgEcrite[250];
   int nH, nW, nTaille, nTaille3;
  
  if (argc != 2) 
     {
       printf("Usage: ImageIn\n"); 
       exit (1) ;
     }

  const char *folder = "./images/";
  const char *extension = ".ppm";
  string s1 = string(folder) + string(argv[1]) + string(extension);
  char* filename1 = stringToCharArray(s1);
   
   sscanf (filename1,"%s",cNomImgLue) ;

   OCTET *ImgIn;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
   nTaille3 = nTaille * 3;
  
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

   for (int i=0; i < nTaille3; i+=3) {
    histoR[ImgIn[i]]++;
    histoG[ImgIn[i+1]]++;
    histoB[ImgIn[i+2]]++;
   }

   for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << histoR[i] << " "<< histoG[i] << " " << histoB[i] << endl;
   }

   free(ImgIn);
   return 1;
}

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "ImageBase.h"
#include "ImageBase.cpp"
using namespace std;

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  int histoR[256] = {0};
  int histoG[256] = {0};
  int histoB[256] = {0};
  int nH, nW, nTaille;
  
  if (argc != 2) {
   printf("Usage: ImageIn namefile \n"); 
   exit (1) ;
  }
   
   sscanf (argv[1],"%s",cNomImgLue);
  
  ImageBase imIn;
  imIn.load(cNomImgLue);

  for(int y = 0; y < imIn.getHeight(); ++y) {
    for(int x = 0; x < imIn.getWidth(); ++x)
     {
        histoR[imIn[y*3][x*3+0]]++;
        histoG[imIn[y*3][x*3+1]]++;
        histoB[imIn[y*3][x*3+2]]++;
     }
   }

   for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << histoR[i] << " "<< histoG[i] << " " << histoB[i] << endl;
   }
   
}

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
  int histo[256] = {0};
  int nH, nW, nTaille;
  
  if (argc != 2) {
   printf("Usage: ImageIn namefile \n"); 
   exit (1) ;
  }
   
   sscanf (argv[1],"%s",cNomImgLue);
  
  ImageBase imIn;
  imIn.load(cNomImgLue);

  for(int x = 0; x < imIn.getHeight(); ++x) {
    for(int y = 0; y < imIn.getWidth(); ++y)
     {
        histo[imIn[x][y]]++;
     }
   }

   for (int i = 0; i < 256; ++i)
   {
     cout << i << " " << histo[i] << endl;
   }
   
}

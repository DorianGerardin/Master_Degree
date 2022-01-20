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
  int isLigne;
  int num;
  
  if (argc != 4) {
   printf("Usage: ImageIn namefile \n"); 
   exit (1) ;
  }

  sscanf (argv[1],"%s",cNomImgLue) ;
  sscanf (argv[2],"%d",&isLigne) ;
  sscanf (argv[3],"%d",&num);

  
  ImageBase imIn;
  imIn.load(cNomImgLue);

  
  if(isLigne == 1) {
    for(int x = 0; x < imIn.getWidth(); ++x) {
      cout << x << " " << int(imIn[num][x]) << endl;
    }
   } else {
    for(int x = 0; x < imIn.getHeight(); ++x) {
      cout << x << " " << int(imIn[x][num]) << endl;
    }
   }
   
}

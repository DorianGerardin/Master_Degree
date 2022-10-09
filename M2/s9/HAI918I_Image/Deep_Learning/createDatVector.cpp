#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <climits>  
#include <algorithm> 
#include <vector>
#include <array>
#include <cmath>
#include <cfloat>
#include "image_ppm.h"
#include "file.h"

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 1) 
  {
     printf("Usage: ./exe\n"); 
     exit (1) ;
  }

  for (int i = 0; i < 102400; ++i)
  {
    cout << 0.0 << endl;
  }
  return 1;
}
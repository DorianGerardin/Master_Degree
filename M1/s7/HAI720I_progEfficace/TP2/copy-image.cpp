#include "image-pnm.h"
#include <string>
#include <iostream>
using namespace std;



using ImagePBM = Image<uint8_t>;
using ImagePGM = Image<uint8_t>;
using ImagePGMf= Image<float>;
using RGB256 = RGB<uint8_t>;
using ImagePPM = Image<RGB256>;


int main(int argc, char** argv){

  if (argc!=2){
    cerr<<"Error: using with parameters: fileIn \n";
    return EXIT_FAILURE;
  }

  // get the 1st argument as the input image filename
  char *fileIn;
  fileIn=argv[1];

  // open the image file through a stream
  ifstream is(fileIn);
  if (!is.is_open()){
    cerr<<"Error: problem opening image "<< fileIn<<endl;
    return EXIT_FAILURE;  
  }
  string  magic_number;
  getline(is,magic_number);
  is.close();
  if (magic_number!= "P1" and magic_number!= "P2" and magic_number!= "P3"){
    cerr<<"Error: image in "<< fileIn<<" does not have the correct format (pbm,pgm,ppm with ASCII encoding)"<<endl;
    return EXIT_FAILURE;  
  }
    
  

  if (magic_number == "P1"){  
    string fileOut(fileIn);
    ImagePBM Img("P1");
    fileOut=fileOut.substr(0,fileOut.find_first_of("."))+string("-out.pbm");
    Img.read(fileIn);
    Img.write(fileOut.c_str());
    cout<<"Image written to "<<fileOut<<endl;
  }
  if (magic_number == "P2"){
    string fileOut(fileIn);
    ImagePGM Img("P2");
    fileOut=fileOut.substr(0,fileOut.find_first_of("."))+string("-out.pgm");
    Img.read(fileIn);   
    Img.write(fileOut.c_str());
    cout<<"Image written to "<<fileOut<<endl;
  }
  if (magic_number == "P3"){
    string fileOut(fileIn);
    ImagePPM Img("P3");
    fileOut=fileOut.substr(0,fileOut.find_first_of("."))+string("-out.ppm");
    Img.read(fileIn);   
    Img.write(fileOut.c_str());
    cout<<"Image written to "<<fileOut<<endl;    
  }




  return 0;
}

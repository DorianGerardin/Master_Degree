#ifndef __IMAGE_PNM_H
#define __IMAGE_PNM_H
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

template<typename T> 
struct RGB {T r,g,b;};


template<typename T>
std::istream& readPixel(std::istream& is, T& pixel){
  uint32_t tmp; is>>tmp; pixel=tmp;
  return is;
}

template<typename T>
std::istream& readPixel(std::istream& is, RGB<T>& pixel){
  uint32_t r,g,b; is>>r>>g>>b;
  pixel.r=r;
  pixel.g=g;
  pixel.b=b;
  return is;
}

template<typename T>
std::ostream& writePixel(std::ostream& os, const T& pixel){
  return os<<(uint32_t) pixel;
}
template<typename T>
std::ostream& writePixel(std::ostream& os, const RGB<T>& pixel){
  return os<<(uint32_t) pixel.r<<" "<<(uint32_t) pixel.g<<" "<<(uint32_t) pixel.b;
}


  
template<typename T>
class Image{
 private:
  std::vector<T> _data; // vector of pixel value
  size_t _row,_col;     // row and column dimension
  std::string _type;    // a string encoding the image format ("P1"=PBM, "P2"=PGM, "P3"=PPM) 
  
 public:
  
  
  Image(std::string type, size_t w=0, size_t h=0) : _type(type), _data(h*w), _row(h), _col(w) {}


  // method for accessing image width and height
  size_t width()  const {return _col;}
  size_t height() const {return _row;}

  // method to access individual pixel in the image
  T&       get(size_t i, size_t j)       {return _data[i*_col+j];}
  const T& get(size_t i, size_t j)const  {return _data[i*_col+j];}

  // method to get access to the underlying image as an array through a pointer
  T*       getData()       {return _data.data();}  
  const T* getData()const  {return _data.data();}


  // method to read the image from a file
  void read(const char * file){    
    std::ifstream is(file);
    std::string control;
    int range_value;
    is>>control;
    if (control!=_type){
      std::cerr<<"file "<<file<<" does not have the correct format.... aborting"<<std::endl;
      return ;
    }
    is>>_col>>_row;
    _data.resize(_row*_col);
    std::cout<<"reading an image of size: "<<_col<<"x"<<_row<<std::endl;

    if (_type != "P1")
      is>>range_value;	
    for (size_t i=0;i<_row;i++){
      for(size_t j=0;j<_col;j++){
	readPixel(is,_data[i*_col+j]);
      }
    }
  }
  
  // method to write the image to a file
  void write(const char * file) const {     
    std::ofstream os(file);
    std::string control;
    os<<_type<<std::endl;
    os<<_col<<" "<<_row<<std::endl;
    if (_type != "P1")
      os<<255<<std::endl;    
    for (size_t i=0;i<_row;i++){
      for(size_t j=0;j<_col;j++){
	writePixel(os,_data[i*_col+j])<<" ";
      }      
      os<<std::endl;
    }
  } 
};

#endif

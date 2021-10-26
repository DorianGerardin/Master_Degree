#include <string>
#include <iostream>
#include "Vec3.h"
#include "Mesh.h"

class Maillage{
 private:
  /*std::vector< Vec3 > vertices; 
  std::vector< Vec3 > normals;
  std::vector< Triangle > triangles;*/
 public:
 	void simplify(unsigned int resolution);
 	void computeCube(Mesh & i_mesh);
  
};
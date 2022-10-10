#include <igl/opengl/glfw/Viewer.h>
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

using namespace Eigen;
using namespace std;

void displayName(string s) {
  cout << s + " : " << endl;
 }

Vector3d getBarycentre(const MatrixXd m) {
  return m.colwise().sum().transpose() / (double) m.rows();
}

MatrixXd convertToSpherical(MatrixXd m, VectorXd bary) {
  MatrixXd res(m.rows(), m.cols());
  for (int i = 0; i < m.rows(); ++i){
    res(i, 0) = sqrt( pow((m(i, 0)-bary(0)), 2) + pow((m(i, 1)-bary(1)), 2) + pow((m(i, 2)-bary(2)), 2));
    res(i, 1) = atan2( (m(i, 1)-bary(1)), (m(i, 0)-bary(0)) );
    res(i, 2) = acos( (m(i, 2)-bary(2)) / res(i, 0) );
  }
  return res;
}

MatrixXd convertToCartesian(MatrixXd m, Vector3d bary) {
  MatrixXd res(m.rows(), m.cols());
  for (int i = 0; i < m.rows(); ++i){
    res(i, 0) = m(i, 0) * cos(m(i, 1)) * sin(m(i, 2)) + bary(0);
    res(i, 1) = m(i, 0) * sin(m(i, 1)) * sin(m(i, 2)) + bary(1);
    res(i, 2) = m(i, 0) * cos(m(i, 2)) + bary(2);
  }
  return res;
}

double getMinRadial(MatrixXd m) {
  return m.col(0).minCoeff();
}


double getMaxRadial(MatrixXd m) {
  return m.col(0).maxCoeff();
}

MatrixXd normalize(MatrixXd m) {
  MatrixXd res(m.rows(), m.cols());
  double minRadial = getMinRadial(m);
  double maxRadial = getMaxRadial(m);
  double length = maxRadial - minRadial;

  for (int i = 0; i < m.rows(); ++i){
    res(i, 0) = abs(m(i, 0) - minRadial) / length;
    res(i, 1) = m(i, 1);
    res(i, 2) = m(i, 2);
  }
  return res;
}

MatrixXd denormalize(MatrixXd m, double min, double max) {
  MatrixXd res(m.rows(), m.cols());
  double length = max - min;
  for (int i = 0; i < m.rows(); ++i){
    res(i, 0) = min + (m(i, 0) * length);
    res(i, 1) = m(i, 1);
    res(i, 2) = m(i, 2);
  }
  return res;
}



int main(int argc, char *argv[])
{
  // Inline mesh of a cube
  const Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
    0.0,0.0,0.0,
    0.0,0.0,1.0,
    0.0,1.0,0.0,
    0.0,1.0,1.0,
    1.0,0.0,0.0,
    1.0,0.0,1.0,
    1.0,1.0,0.0,
    1.0,1.0,1.0).finished();
  const Eigen::MatrixXi F = (Eigen::MatrixXi(12,3)<<
    1,7,5,
    1,3,7,
    1,4,3,
    1,2,4,
    3,8,7,
    3,4,8,
    5,7,8,
    5,8,6,
    1,5,6,
    1,6,2,
    2,6,8,
    2,8,4).finished().array()-1;

  IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]", "[", "]");
  std::string sep = "\n----------------------------------------\n";


  Vector3d barycentre = getBarycentre(V);

  MatrixXd sphericalCoordsMatrix = convertToSpherical(V, barycentre);

  MatrixXd cartesianCoordsMatrix = convertToCartesian(sphericalCoordsMatrix, barycentre);

  MatrixXd normalizedV = normalize(V);

  double minRadial = getMinRadial(V);
  double maxRadial = getMaxRadial(V);
  MatrixXd denormalizedV = denormalize(normalizedV, minRadial, maxRadial);

  displayName("sphericalCoordsMatrix");
  cout << sphericalCoordsMatrix.format(CleanFmt) << sep;

  displayName("cartesianCoordsMatrix");
  cout << cartesianCoordsMatrix.format(CleanFmt) << sep;

  displayName("normalizedV");
  cout << normalizedV.format(CleanFmt) << sep;

  displayName("denormalizedV");
  cout << denormalizedV.format(CleanFmt) << sep;


  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V, F);
  viewer.data().set_face_based(true);
  viewer.launch();
}

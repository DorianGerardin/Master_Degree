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
#include <igl/readOFF.h>

using namespace Eigen;
using namespace std;

IOFormat CleanFmt(4, 0, ", ", "\n", "[", "]", "[", "]");
std::string sep = "\n----------------------------------------\n";

void displayName(string s) {
  cout << s + " : " << endl;
}

MatrixXd initMatrixFrom(MatrixXd m) {
  MatrixXd res(m.rows(), m.cols());
  for (int i = 0; i < m.rows(); ++i)
  {
    res(i, 0) = m(i, 0);
    res(i, 1) = m(i, 1);
    res(i, 2) = m(i, 2);
  }
  return res;
}

Vector3d getBarycentre(const MatrixXd m) {
  return m.colwise().sum().transpose() / (double) m.rows();
}

void convertToSpherical(MatrixXd &m, VectorXd bary) {
  for (int i = 0; i < m.rows(); ++i){
    double rho = sqrt( pow((m(i, 0)-bary(0)), 2) + pow((m(i, 1)-bary(1)), 2) + pow((m(i, 2)-bary(2)), 2));
    double theta = atan2( (m(i, 1)-bary(1)), (m(i, 0)-bary(0)) );
    double phi = acos( (m(i, 2)-bary(2)) / rho );
    m(i, 0) = rho;
    m(i, 1) = theta;
    m(i, 2) = phi;
  }
}

void convertToCartesian(MatrixXd &m, Vector3d bary) {
  for (int i = 0; i < m.rows(); ++i){
    double rho = m(i, 0) * cos(m(i, 1)) * sin(m(i, 2)) + bary(0);
    double theta = m(i, 0) * sin(m(i, 1)) * sin(m(i, 2)) + bary(1);
    double phi = m(i, 0) * cos(m(i, 2)) + bary(2);
    m(i, 0) = rho;
    m(i, 1) = theta;
    m(i, 2) = phi;
  }
}

double getMinRadialFromMatrix(MatrixXd m) {
  return m.col(0).minCoeff();
}

double getMaxRadialFromMatrix(MatrixXd m) {
  return m.col(0).maxCoeff();
}

double getMinRadialFromBin(MatrixXd m, vector<unsigned int> bin) {
  double min = DBL_MAX;
  for (int i = 0; i < bin.size(); ++i){
    if(m(bin[i], 0) < min) min = m(bin[i], 0);
  } return min;
}

double getMaxRadialFromBin(MatrixXd m, vector<unsigned int> bin) {
  double max = DBL_MIN;
  for (int i = 0; i < bin.size(); ++i){
    if(m(bin[i], 0) > max) max = m(bin[i], 0);
  } return max;
}

void normalizeMatrix(MatrixXd &m) {
  double minRadial = getMinRadialFromMatrix(m);
  double maxRadial = getMaxRadialFromMatrix(m);
  double length = maxRadial - minRadial;

  for (int i = 0; i < m.rows(); ++i){
    m(i, 0) = abs(m(i, 0) - minRadial) / length;
    m(i, 1) = m(i, 1);
    m(i, 2) = m(i, 2);
  }
}

void denormalizeMatrix(MatrixXd &m, double min, double max) {
  double length = max - min;
  for (int i = 0; i < m.rows(); ++i){
    m(i, 0) = min + (m(i, 0) * length);
    m(i, 1) = m(i, 1);
    m(i, 2) = m(i, 2);
  }
}

void normalize(MatrixXd &m, vector<unsigned int> bin) {
  double minRadial = getMinRadialFromBin(m, bin);
  double maxRadial = getMaxRadialFromBin(m, bin);
  double length = maxRadial - minRadial;
  for (int i = 0; i < bin.size(); ++i){
    m(bin[i], 0) = abs(m(bin[i], 0) - minRadial) / length;
  }
}

void denormalize(MatrixXd &m, vector<unsigned int> bin, double min, double max) {
  double length = max - min;
  for (int i = 0; i < bin.size(); ++i){
    m(bin[i], 0) = min + (m(bin[i], 0) * length);
  }
}

vector<vector<unsigned int>> histogram(MatrixXd m, unsigned int k) {
  vector<vector<unsigned int>> histogram;
  histogram.resize(k);
  double minRadial = getMinRadialFromMatrix(m);
  double maxRadial = getMaxRadialFromMatrix(m);
  for (int i = 0; i < k; ++i) {
    for (int j = 0; j < m.rows(); ++j) {
      double Bmin = minRadial + ((maxRadial-minRadial)/k) * i;
      double Bmax = minRadial + ((maxRadial-minRadial)/k) * (i+1);
      if(m(j, 0) > Bmin && m(j, 0) < Bmax) {
        histogram[i].push_back(j);
      }
    }
  }
  return histogram;
}

double getMeanOfBin(MatrixXd &m, vector<unsigned int> bin) {
  double sum = 0.0;
  size_t k = bin.size();
  for (int i = 0; i < k; ++i){
    sum += m(bin[i], 0);
  }
  return sum / (double)k;
}

void binToPowerOf(MatrixXd &m, vector<unsigned int> bin, double k) {
  for (int i = 0; i < bin.size(); ++i){
    m(bin[i], 0) = pow( m(bin[i], 0), k );
  }
}

void insertion(MatrixXd &m, bool *message, unsigned int n, double k, double alpha) {
  Vector3d barycentre = getBarycentre(m);
  convertToSpherical(m, barycentre);
  vector<vector<unsigned int>> bins = histogram(m, n);
  double deltaK = 0.001;
  for (int i = 0; i < n; ++i) {
    double minRadial = getMinRadialFromBin(m, bins[i]);
    double maxRadial = getMaxRadialFromBin(m, bins[i]);
    normalize(m, bins[i]);
    double kCopy = k;
    double mean = getMeanOfBin(m, bins[i]);
    if(message[i]) {
      while(mean < (0.5 + alpha)) {
        kCopy -= deltaK;
        binToPowerOf(m, bins[i], kCopy);
        mean = getMeanOfBin(m, bins[i]);
      }
    } 
    else {
      while(mean > (0.5 - alpha)) {
        kCopy += deltaK;
        binToPowerOf(m, bins[i], kCopy);
        mean = getMeanOfBin(m, bins[i]);
      }
    }
    denormalize(m, bins[i], minRadial, maxRadial);
  }
  convertToCartesian(m, barycentre);
}

void extraction(MatrixXd &m, vector<bool> &message, unsigned int n, double alpha) {
  message.resize(n);
  Vector3d barycentre = getBarycentre(m);
  convertToSpherical(m, barycentre);
  vector<vector<unsigned int>> bins = histogram(m, n);
  for (int i = 0; i < n; ++i) {
    double minRadial = getMinRadialFromBin(m, bins[i]);
    double maxRadial = getMaxRadialFromBin(m, bins[i]);
    normalize(m, bins[i]);
    double mean = getMeanOfBin(m, bins[i]);
    mean > 0.5 ? message[i] = 1 : message[i] = 0;
    denormalize(m, bins[i], minRadial, maxRadial);
  }
  convertToCartesian(m, barycentre);
}

double RMSE(MatrixXd &m, MatrixXd &t) {
  double eqm = 0.0;
  for(unsigned int i = 0 ; i < m.rows() ; i++){
    for(unsigned int j = 0 ; j < m.cols(); j++){
      eqm += pow(m(i,j) - t(i,j),2);
    }
  }
  eqm/=(m.rows() * m.cols());
  return sqrt(eqm);
}

int main(int argc, char *argv[])
{
  // Inline mesh of a cube
  // const Eigen::MatrixXd V= (Eigen::MatrixXd(8,3)<<
  //   0.0,0.0,0.0,
  //   0.0,0.0,1.0,
  //   0.0,1.0,0.0,
  //   0.0,1.0,1.0,
  //   1.0,0.0,0.0,
  //   1.0,0.0,1.0,
  //   1.0,1.0,0.0,
  //   1.0,1.0,1.0).finished();
  // const Eigen::MatrixXi F = (Eigen::MatrixXi(12,3)<<
  //   1,7,5,
  //   1,3,7,
  //   1,4,3,
  //   1,2,4,
  //   3,8,7,
  //   3,4,8,
  //   5,7,8,
  //   5,8,6,
  //   1,5,6,
  //   1,6,2,
  //   2,6,8,
  //   2,8,4).finished().array()-1;

  MatrixXd V_bunny;
  MatrixXi F_bunny;

  igl::readOFF("../tp/bunny.off", V_bunny, F_bunny);

  MatrixXd V_bunny_tattoo = initMatrixFrom(V_bunny);

  unsigned int n = 25;
  bool message[n] = {0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1};
  double k = 1.0;
  double alpha = 0.45;
  insertion(V_bunny_tattoo, message, n, k, alpha);

  double rmse = RMSE(V_bunny, V_bunny_tattoo);
  cout << "RMSE : " << rmse << endl;

  vector<bool> extractedMessage;
  extraction(V_bunny_tattoo, extractedMessage, n, alpha);
  cout << endl;
  cout << "Message extrait : " << endl;
  for (int i = 0; i < n; ++i)
  {
    bool value = extractedMessage[i];
    if(i == n-1) cout << value << endl << endl;
    else cout << value << ",";
  }

  // Plot the mesh
  igl::opengl::glfw::Viewer viewer;
  viewer.data().set_mesh(V_bunny_tattoo, F_bunny);
  viewer.data().set_face_based(true);
  viewer.launch();
}

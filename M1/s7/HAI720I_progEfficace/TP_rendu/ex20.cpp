#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "eval-perf.cpp"
#include <cmath>

using namespace std;

const size_t N = 100000;

EvalPerf PE;

// --------------- Quest 4 --------------- //

void MatrixTrans_naif(double *A, double *B, int n) {

	size_t i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            B[i*n+j] = A[j*n+i];

}

void copy(double *A, double *B, int n) {
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			A[i*n+j] = B[i*n+j];
		}
	}
}

// --------------- Quest 5 --------------- //

void MatrixTrans_Block(double *A, double *B, int n, int b) {

	for (size_t k = 0; k < n; k+=b)
	{
		for (size_t l = 0; l < n; l+=b)
		{
			for (size_t i = 0; i < b; ++i)
			{
				for (size_t j = 0; j < b; ++j)
				{
					B[(i+k)*n+(j+l)] = A[(j+l)*n+(i+k)];
				}
			}
		}
	}
}

// --------------- Quest 6 --------------- //

void printSubMatrix(const double* M, size_t m, int n, int s) {
	for (size_t i = 0; i < m; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			std::cout << M[i*s+j] << " ";
		}
		std::cout << endl;
	}
}

void MatrixTrans_obliv(size_t m, size_t n, double *A, size_t sa, double *B, size_t sb) {

	if(m == 1 && n == 1) {
		B[0] = A[0];
	}
	else if (m == 1) {
		for (int i = 0; i < n; ++i)
		{
			B[i] = A[i];
		}
	} else if (n == 1) {
		for (int i = 0; i < m; ++i)
		{
			B[i] = A[i];
		}
	} else {
		//B0 = AT0
		MatrixTrans_obliv(m/2, n/2, A, sa, B, sb);
		//B1 = AT2
		MatrixTrans_obliv(m/2, n/2, A + (sa*(m/2)), sa, B + n/2, sb);
		//B2 = AT1
		MatrixTrans_obliv(m-m/2, n-n/2, A + n/2, sa, B + (sb*m/2), sb);
		//B3 = AT3
		MatrixTrans_obliv(m-m/2, n-n/2, A + (sa*(m/2)+(n/2)), sa, B + (sb*(m/2)+(n/2)), sb);
	}

}

void MatrixTrans_obliv2(size_t m, size_t n, double *A, size_t sa, double *B, size_t sb, int minAppels) {

	if (n <= minAppels){
        MatrixTrans_naif(A, B, n);
    }
	else if (m == 1) {
		for (int i = 0; i < n; ++i)
		{
			B[i] = A[i];
		}
	} else if (n == 1) {
		for (int i = 0; i < m; ++i)
		{
			B[i] = A[i];
		}
	} else {
		//B0 = AT0
		MatrixTrans_obliv(m/2, n/2, A, sa, B, sb);
		//B1 = AT2
		MatrixTrans_obliv(m/2, n/2, A + (sa*(m/2)), sa, B + n/2, sb);
		//B2 = AT1
		MatrixTrans_obliv(m-m/2, n-n/2, A + n/2, sa, B + (sb*m/2), sb);
		//B3 = AT3
		MatrixTrans_obliv(m-m/2, n-n/2, A + (sa*(m/2)+(n/2)), sa, B + (sb*(m/2)+(n/2)), sb);
	}

}

void printPE(std::string name, int nbInstructions) {
	std::cout << " \t " << name << " : " <<std::endl;

  	std::cout<<"nbr cycles : " << PE.nb_cycles()<<std::endl;
	std::cout<<"nbr secondes : " << PE.second().count()<<std::endl;
	std::cout<<"nbr millisecondes : "<< PE.milisecond().count()<<std::endl;
	std::cout<<"CPI = "<< PE.CPI(N*nbInstructions)<<std::endl;

	std::cout<<""<<std::endl;
}

int main() {

	// --------------- Quest 4 --------------- //
	
/*	int n = 455;
	double B[n*n], A[n*n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			A[i*n+j] = double(rand()%1000+1);
		}
	}

  	PE.start(); 
  	for (int i = 0; i < N; ++i)
  	{
  		MatrixTrans_naif(A, B, n);
  	}
  	PE.stop() ;
	printPE("Transpose matrix", n*n);
*/


/*	
  	PE.start(); 
  	for (int i = 0; i < N; ++i)
  	{
  		copy(A, B, n);
  	}
  	PE.stop() ;
	printPE("Copy matrix", n*n);

	// --------------- Quest 5 --------------- //

	int n = 128;
	double B[n*n], A[n*n];

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			A[i*n+j] = double(rand()%1000+1);
		}
	}

  	PE.start(); 
  	for (size_t i = 0; i < N; ++i)
  	{
  		MatrixTrans_naif(A, B, n);
  	}
  	PE.stop() ;
  	printPE("Transpose matrix naif", n*n);


  	PE.start(); 
  	int b = 16;
  	for (size_t i = 0; i < N; ++i)
  	{
  		MatrixTrans_Block(A, B, n, b);
  	}
  	PE.stop() ;
  	printPE("Transpose with blocks", n*n);

	// --------------- Quest 6 --------------- //

  	PE.start(); 
  	for (size_t i = 0; i < N; ++i)
  	{
  		MatrixTrans_obliv(n, n, A, n, B, n);
  	}
  	PE.stop() ;
  	printPE("Transpose with Recursive", pow(log2(n), 4) * 4);


  	PE.start(); 
  	int minAppels = log2(n)*n;
  	for (size_t i = 0; i < N; ++i)
  	{
  		MatrixTrans_obliv2(n, n, A, n, B, n, minAppels);
  	}
  	PE.stop() ;
	printPE("Transpose with Recursive min appels", pow(log2(n), 4) * 4);

}
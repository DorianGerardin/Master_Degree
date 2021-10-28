#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "eval-perf.cpp"
#include <cmath>

using namespace std;

#define n 455

int N = 50000;

void MatrixTrans_naif(double A[][n], double B[][n]) {

	int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            B[i][j] = A[j][i];

}

void copy(double A[][n], double B[][n]) {
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			A[i][j] = B[i][j];
		}
	}
}

int main() {

	double B[n][n], A[n][n];

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			A[i][j] = sqrt((double)rand() / RAND_MAX);
		}
	}

	EvalPerf PE;
  	PE.start(); 

  	for (int i = 0; i < N; ++i)
  	{
  		MatrixTrans_naif(A, B);
  	}

  	PE.stop() ;

  	std::cout << " \t Transpose matrix : " <<std::endl;

  	std::cout<<"nbr cycles : " << PE.nb_cycles()<<std::endl;
	std::cout<<"nbr secondes : " << PE.second().count()<<std::endl;
	std::cout<<"nbr millisecondes : "<< PE.milisecond().count()<<std::endl;
	std::cout<<"CPI = "<< PE.CPI(N)<<std::endl;

	std::cout<<""<<std::endl;

  	//---------------------------------------------------------------------//

	EvalPerf PE2;
  	PE2.start(); 

  	for (int i = 0; i < N; ++i)
  	{
  		copy(A, B);
  	}

  	PE2.stop() ;

  	std::cout << "\t Easy code : " <<std::endl;

  	std::cout<<"nbr cycles : " << PE2.nb_cycles()<<std::endl;
	std::cout<<"nbr secondes : " << PE2.second().count()<<std::endl;
	std::cout<<"nbr millisecondes : "<< PE2.milisecond().count()<<std::endl;
	std::cout<<"CPI = "<< PE2.CPI(N)<<std::endl;



}
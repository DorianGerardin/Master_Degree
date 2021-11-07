#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "eval-perf.cpp"
#include <cmath>
#include <algorithm>

using namespace std;

const size_t N = 100000;

EvalPerf PE;

void printPE(std::string name, int nbInstructions) {
	std::cout << " \t " << name << " : " <<std::endl;

  	std::cout<<"nbr cycles : " << PE.nb_cycles()<<std::endl;
	std::cout<<"nbr secondes : " << PE.second().count()<<std::endl;
	std::cout<<"nbr millisecondes : "<< PE.milisecond().count()<<std::endl;
	std::cout<<"CPI = "<< PE.CPI(N*nbInstructions)<<std::endl;

	std::cout<<""<<std::endl;
}

void counting_sort_naif(size_t *R, const size_t *T, size_t n, size_t min, size_t max) {

	size_t tabComptage[max+1] = {0};

	//histogramme des clés
	for (int i = 0; i < n; ++i)
	{
		tabComptage[T[i]+1]++;	
	}

	//somme prefixe
	for (int i = 1; i < max+1; ++i)
	{
		tabComptage[i] += tabComptage[i-1];
	}

	for (int i = 0; i < n; ++i)
	{
		R[tabComptage[T[i]]] = T[i];
		tabComptage[T[i]]++;
	}
}

void counting_sort_bucket(size_t *R, const size_t *T, size_t n, size_t min, size_t max, size_t m) {

	size_t bucket[m+1] = {0};

	for (int i = 0; i < n; ++i)
	{
		int q = (m*T[i]+1)/max;
		bucket[q]++;
	}

	//somme prefixe
	for (int i = 1; i < m+1; ++i)
	{
		bucket[i] += bucket[i-1];
	}

	size_t bucketedInput[n] = {0};
	for (int i = 0; i < n; ++i)
	{
		int q = (m*T[i]+1)/max;
		bucketedInput[bucket[q]] = T[i];
		bucket[q]++;
	}
	counting_sort_naif(R,bucketedInput,n, min, max);
}

void counting_sort_naif_upgrade(size_t *R, const size_t *T, size_t n, size_t min, size_t max) {

	size_t tabComptage[max+1] = {0};
	size_t x = 0;
	size_t j[1] = {0};

	//histogramme des clés
	for (int i = 0; i < n; ++i)
	{
		tabComptage[T[i]]++;	
	}

	for (int i = 0; i < max+1; ++i)
	{
		for (j[0] = 0; j[0] < tabComptage[i]; ++j[0])
		{
			R[x++] = i;
		}
	}
}

int main() {

	int n = 6;
	size_t T[n] = {1, 27, 3, 1, 3, 12};
	size_t R[n];
	size_t R2[n];
	size_t R3[n];

/*	PE.start(); 
	for (size_t i = 0; i < N; ++i)
  	{ 
		counting_sort_naif(R, T, n, 1, 27);
	}
	PE.stop();
  	printPE("Tri naif", n*n);
	for (int i = 0; i < n; ++i)
	{
		cout << "R[" << i << "] = " << R[i] << endl;
	}*/

	PE.start();
	/*for (size_t i = 0; i < N; ++i)
  	{*/
		counting_sort_bucket(R2, T, n, 1, 27, 4);
	//}
	PE.stop();
	printPE("Tri bucket", n*n);
	for (int i = 0; i < n; ++i)
	{
		cout << "R2[" << i << "] = " << R2[i] << endl;
	}

/*	PE.start();
	for (int i = 0; i < N; ++i)
	{
		counting_sort_naif_upgrade(R3, T, n, 1, 27);
	}
	PE.stop();
	printPE("Tri naif ameliore", n);
	for (int i = 0; i < n; ++i)
	{
		cout << "R3[" << i << "] = " << R3[i] << endl;
	}*/

/*	PE.start();
	for (int i = 0; i < N; ++i)
	{
		std::sort(T, (T+n));
	}
	PE.stop();
	printPE("sort bibliotheque", n);
	for (int i = 0; i < n; ++i)
	{
		cout << "T[" << i << "] = " << T[i] << endl;
	}*/
}
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "eval-perf.cpp"
#include <cmath>
#include <algorithms>

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

void counting_sort_bucket(size_t *R, const size_t *T, size_t n, size_t min, size_t max) {



}

int main() {

	int n = 5;
	size_t T[n] = {1, 27, 3, 1, 3};
	size_t R[n];

	counting_sort_naif(R, T, n, 1, 27);

	for (int i = 0; i < n; ++i)
	{
		cout << "R[" << i << "] = " << R[i] << endl;
	}
}
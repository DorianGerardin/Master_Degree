#include "evalPerf.h"
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <x86intrin.h>


void EvalPerf::start() {

	this->startTime = 
    std::chrono::system_clock::now();

	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	this->nbCyclesAtStart = ((uint64_t)hi << 32) | lo;
}
void EvalPerf::stop() {

	this->endTime = 
    std::chrono::system_clock::now();

	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	this->nbCyclesAtEnd = ((uint64_t)hi << 32) | lo;
}
int EvalPerf::nbCycles() {
	return this->nbCyclesAtEnd-this->nbCyclesAtStart;
}
float EvalPerf::second() {
	return EvalPerf::millisecond()/1000;
}
int EvalPerf::millisecond() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(this->endTime - this->startTime).count();
}
// int CPI(int N) {

// }



void ma_fonction(int N) {
	int a = 0;
	for (int i = 0; i < N; ++i)
	{
		a += a*i+2*i;
	}
}

int main(int argc, char *argv[]) {

	if (argc != 2){
	    printf("mauvais nombre de parametres \n");
	    exit(0);
  	}

	EvalPerf PE;

	PE.start();
	ma_fonction(atoi(argv[1]));
	PE.stop();
	std::cout<< "nbr cycles : " << PE.nbCycles() << std::endl;
	std::cout<< "nbr secondes : " << PE.second() << std::endl;
	std::cout<< "nbr ms : " << PE.millisecond() << std::endl;
	//std::cout<< "CPI = " << PE.CPI(N) << std::endl;

	return 0;

}
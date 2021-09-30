#include "immintrin.h"
#include <iostream>
#include <vector>

int reduce(std::vector<int> arr) {

	int res = 1;
	int size = arr.size();
	for (int i = 0; i < size; ++i)
	{
		res *= arr[i];
	}
	return res;

}

int reduce4(std::vector<int> arr) {

	int res = 1;
	int size = arr.size();
	for (int i = 0; i < size-3; i+=4)
	{
		res *= arr[i];
		res *= arr[i+1];
		res *= arr[i+2];
		res *= arr[i+3];
	}
	return res;

}

int reduceSIMD(std::vector<int> arr) {

	__m256d res = _mm256_set1_pd(1);
	int trueResult[4];
	int size = arr.size();
	__m256d aux;
	for (int i = 0; i < size-3; i+=4)
	{
		aux = _mm256_load_pd(arr.data() + i);
		res = _mm256_mul_pd(res, aux);
	}
	_mm256_store_pd(trueResult);
	return trueResult[0]*trueResult[1]*trueResult[2]*trueResult[3];

}

int main(int argc, char **argv) {
	return 0;
}




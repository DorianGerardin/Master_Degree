#include "immintrin.h"
#include <iostream>
#include <vector>
#include <cfloat>
#include <cmath>
#include <algorithm>

double reduce(std::vector<double> arr) {

	double res = 0;
	int size = arr.size();
	for (int i = 0; i < size; ++i)
	{
		res += arr[i];
	}
	return res;

}

double reduce4(std::vector<double> arr) {

	double res = 0;
	int size = arr.size();
	for (int i = 0; i < size-3; i+=4)
	{
		res += arr[i];
		res += arr[i+1];
		res += arr[i+2];
		res += arr[i+3];
	}
	return res;

}

double reduceSIMD(std::vector<double> arr) {

	__m256d res = _mm256_set1_pd(0);
	double trueResult[4];
	int size = arr.size();
	__m256d aux;
	for (int i = 0; i < size-3; i+=4)
	{
		aux = _mm256_load_pd(arr.data() + i);
		res = _mm256_add_pd(res, aux);
	}
	_mm256_store_pd(trueResult, res);
	return trueResult[0]+trueResult[1]+trueResult[2]+trueResult[3];

}

int32_t reduce8SIMD(std::vector<int32_t> arr) {

	__m256i res = _mm256_set1_epi32(0);
	int32_t trueResult[8];
	int size = arr.size();
	__m256i aux;
	for (int i = 0; i < size-7; i+=8)
	{
		aux = _mm256_loadu_si256((__m256i*)(arr.data() + i));
		res = _mm256_add_epi32(res, aux);
	}
	_mm256_storeu_si256((__m256i*)trueResult, res);
	return trueResult[0]+trueResult[1]+trueResult[2]+trueResult[3]+trueResult[4]+trueResult[5]+trueResult[6]+trueResult[7];

}

float minFloatArr(std::vector<float> arr) {

	__m256 res = _mm256_set1_ps(FLT_MAX);
	float trueResult[8];
	int size = arr.size();
	__m256 aux;
	for (int i = 0; i < size-7; i+=8)
	{
		aux = _mm256_load_ps(arr.data() + i);
		res = _mm256_min_ps(res, aux);
	}
	_mm256_store_ps(trueResult, res);
	return fminf(trueResult[0], fminf(trueResult[1], fminf(trueResult[2], fminf(trueResult[3], fminf(trueResult[4], fminf(trueResult[5], fminf(trueResult[6], trueResult[7])))))));

}

int main(int argc, char **argv) {
	std::vector<double> v = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	std::vector<float> vf = {6,4,9,3,1,-6,-12,78,6,4,9,3,1,-6,-24,78};
	printf("result reduce: %f\n", reduce(v));
	printf("result reduce 4 : %f\n", reduce4(v));
	printf("result SIMD: %f\n", reduceSIMD(v));
	printf("result min in array: %f\n", minFloatArr(vf));
	std::vector<int32_t> v2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	printf("result reduce SIMD 8 : %i\n", reduce8SIMD(v2));
}




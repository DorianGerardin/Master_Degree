#include <iostream>
#include "immintrin.h"
#include <vector>
#include <cfloat>
#include <cmath>
#include <algorithm>

//ILP

void matmult(double *C, const double *A, const double *B, size_t n) {
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			C[i][j] = 0;
			for (int k = 0; k < n; ++k)
			{
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

//SIMD (Ne fonctionne pas, je ne comprends pas comment faire)

void matmultSIMD(double *C, const double *A, const double *B, size_t n) {
	__m256d res = _mm256_set1_pd(0);
	__m256d auxA, auxB, res;
	for (size_t i = 0; i < n-3; i+=4)
	{
		for (size_t j = 0; j < n-3; j+=4)
		{
			C[i][j] = 0;
			for (int i = 0; i < n-3; ++i)
			{
				auxA = _mm256_loadu_pd(A[i][j]);
				auxB = _mm256_loadu_pd(A[i][j]); // fonctionne pas
			}
		}
		
	}
	_mm256_store_pd(C, res);
	return 0;
}

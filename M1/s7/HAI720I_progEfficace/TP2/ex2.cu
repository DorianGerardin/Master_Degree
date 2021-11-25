#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

#define cudaCheckError() {                                                                       \
        cudaError_t e=cudaGetLastError();                                                        \
        if(e!=cudaSuccess) {                                                                     \
            printf("Cuda failure %s:%d: '%s'\n",__FILE__,__LINE__,cudaGetErrorString(e));        \
            exit(EXIT_FAILURE);                                                                  \
        }                                                                                        \
    }

__global__ void vectorAddition(float* C, const float* A, const float* B, size_t n) {
  int i=blockIdx.x*blockDim.x+threadIdx.x;
  if(i < n){
    C[i]=A[i]+B[i];
  }
}

void vectorFloatSum(float* C, float* A, float* B, int n){
  for(int i = 0; i < n; i++){
    C[i] = A[i] + B[i];
  }
}


void prepareCallVecFloatSum(int nbVal){

  float* A = (float*) malloc(nbVal*sizeof(float));
  float* B = (float*) malloc(nbVal*sizeof(float));
  float* C = (float*) malloc(nbVal*sizeof(float));

  for(int i = 0; i < nbVal; i++){
    A[i] = (float) (rand() % 65535);
    B[i] = (float) (rand() % 65535);
  }

  vectorFloatSum(C, A, B, nbVal);

  for (int i = 0; i < nbVal; i++)
  {
    cout << C[i] << endl;;
  }

  free(A);
  free(B);
  free(C);
}


void vectorFloatSumCUDA(float *h_A, float *h_B, float *h_C, int n){
  int threads=128;
  int blocks=(n+threads-1)/threads;
  int size = n * sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMallocManaged(&d_A,size);
  cudaMallocManaged(&d_B,size);
  cudaMallocManaged(&d_C,size);

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

  vectorAddition<<<blocks,threads>>>(d_C, d_A, d_B, n);
  cudaDeviceSynchronize();

  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  cudaCheckError();
  
}

void CUDAprepareCallVecFloatSum(){
  int N=1024;
  float* A = (float*) malloc(1024*sizeof(float));
  float* B = (float*) malloc(1024*sizeof(float));
  float* C = (float*) malloc(1024*sizeof(float));

  for(int i = 0; i < N; i++){
    A[i] = (float)i;
    B[i] = (float)(N-i);
  }
  vectorFloatSumCUDA(A,B,C,N);
  for(int i=0;i<10;i++){
    printf("%f\n",C[i]);
  }

  free(A);
  free(B);
  free(C);
}

__global__ void matrixMulKernel(float* M, const float* N, const float* P, int width) {
  int row = blockIdx.y*blockDim.y+threadIdx.y;
  int col = blockIdx.x*blockDim.x+threadIdx.x;
  if((row < width) && (col < width)) {
    float Pvalue = 0;
    // each thread computes one element of the block sub-matrix
    for (int k = 0; k < width; ++k)
    {
      Pvalue += M[row*width+k]*N[k*width+col];
    }
    P[row*width+col] = Pvalue;
  }
}

void matrixMulCUDA(float *h_A, float *h_B, float *h_C, int n){
  int threads=128;
  int blocks=(n+threads-1)/threads;
  int size = n * sizeof(float);
  float *d_A, *d_B, *d_C;

  cudaMallocManaged(&d_A,size);
  cudaMallocManaged(&d_B,size);
  cudaMallocManaged(&d_C,size);

  cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

  matrixMulKernel<<<blocks,threads>>>(d_C, d_A, d_B, n);
  cudaDeviceSynchronize();

  cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

  cudaFree(d_A);
  cudaFree(d_B);
  cudaFree(d_C);

  cudaCheckError();
  
}


int main() {
  prepareCallVecFloatSum(1024);
  CUDAprepareCallVecFloatSum();
  return 0;
}

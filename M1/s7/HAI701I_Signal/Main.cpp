#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
#include <cstring>
#include "Wave.cpp"

using namespace std;

double unsignedCharToDouble(unsigned char c) {
	return ((double)c/127.5)-1.0;
}

unsigned char doubleToUnsignedChar(double d) {
	return (unsigned char)floor(127.5*(d+1.0));
}

void char2Real(char *data8, double *dataR, int n) {
	int i;
	for (int i = 0; i < n; ++i)
	{
		dataR[i] = ((double)data8[i]/127.5)-1.0;
	}
}

void real2Char(double *dataR, char *data8, int n) {

	int i;
	for (int i = 0; i < n; ++i)
	{
		if (dataR[i] > 1.0) 
		{
			data8[i] = 255;
		} else {
			if (dataR[i] < -1)
			{
				data8[i] = 0;
			} else {
				data8[i] = (char) floor(127.5*(dataR[i]+1.0));
			}
		}
	}
}

void real2CharN(double *rx, double *ix, unsigned char *cx, int N) {

	int i;
	double val, vmax, vmin;
	vmax = rx[0]*rx[0] + ix[0]*ix[0];
	vmax = sqrt(vmax); vmin = vmax;
	for (int i = 0; i < N; ++i)
	{
		val = rx[i]*rx[i] + ix[i]*ix[i];
		val = sqrt(val);
		vmin = vmin > val ? val : vmin;
		vmax = vmax < val ? val : vmax;
	}
	for (int i = 0; i < N; ++i)
	{
		val = rx[i]*rx[i] + ix[i]*ix[i];
		val = sqrt(val);
		val = (val-vmin) / (vmax-vmin);
		cx[i] = (char)floor(255*val);
	}
}

void DFT(double *signal, double *partieReelle, double *partieImaginaire, int N) {

	double pi2 = 2*M_PI;
	for (unsigned int k = 0; k < N; ++k)
	{
		double alpha = (double)k/N;
		double ak = 0;
		double bk = 0;
		for (unsigned int n = 0; n < N; ++n)
		{
			ak += signal[n] * cos(pi2*(alpha*n));
			bk -= signal[n] * sin(pi2*(alpha*n));
		}
		partieReelle[k] = ak;
		partieImaginaire[k] = bk;
	}

}

void IDFT(double *signal, double *partieReelle, double *partieImaginaire, int N) {

	double ak = 0;
	double bk = 0;
	double pi2 = 2*M_PI;
	for (unsigned int k = 0; k < N; ++k)
	{
		double alpha = (double)k/N;
		for (unsigned int n = 0; n < N; ++n)
		{
			signal[k] 
		}
	}

}

void Trouver_m(int N) {
	int n = 0;
	int M = 1;
	while (M<N) {
		m++;
		M*=2;
	}
}


void FFT(int dir,int m,double *x,double *y)
{
	int n,i,i1,j,k,i2,l,l1,l2;
	double c1,c2,tx,ty,t1,t2,u1,u2,z;
	
	/* Calculate the number of points */
	n = 1;
	for (i=0;i<m;i++) 
		n *= 2;
	
	/* Do the bit reversal */
	i2 = n >> 1;
	j = 0;
	for (i=0;i<n-1;i++) {
		if (i < j) {
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}
		k = i2;
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		j += k;
	}
	
	/* Compute the FFT */
	c1 = -1.0; 
	c2 = 0.0;
	l2 = 1;
	for (l=0;l<m;l++) {
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0; 
		u2 = 0.0;
		for (j=0;j<l1;j++) {
			for (i=j;i<n;i+=l2) {
				i1 = i + l1;
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];
				x[i1] = x[i] - t1; 
				y[i1] = y[i] - t2;
				x[i] += t1;
				y[i] += t2;
			}
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		c2 = sqrt((1.0 - c1) / 2.0);
		if (dir == 1) 
			c2 = -c2;
		c1 = sqrt((1.0 + c1) / 2.0);
	}
	
	/* Scaling for forward transform */
	if (dir == 1) {
		for (i=0;i<n;i++) {
			x[i] /= n;
			y[i] /= n;
		}
	}
}


int main(int argc, char *argv[])
{
	const char *folder = "sounds/";
	const char *extension = ".wav";
	string s = string(folder) + string(argv[1]) + string(extension);
	int filenameSize = s.length();
	char filename[filenameSize+1];
	strcpy(filename, s.c_str());
	
	double nbSec = 0.2;
	int sampling_freq = 44100;
	short channels_nb = 1;
	long int data_nb = floor(nbSec*sampling_freq);
	int freq_note = 440;
	unsigned char data8[data_nb];

	double xk;
	double signal[data_nb];
	double partieReelle[data_nb];
	double partieImaginaire[data_nb];
	for (unsigned int i = 0; i < data_nb; ++i)
	{
		xk = sin(2*M_PI*freq_note*i*(1/(double)sampling_freq));
		signal[i] = xk;
		/*unsigned char ucXk = doubleToUnsignedChar(xk); // Note La
		*(data8+i) = ucXk; */
	}

	DFT(signal, partieReelle, partieImaginaire, data_nb);
	real2CharN(partieReelle, partieImaginaire, data8, data_nb);

	Wave *laWave = new Wave(data8, data_nb, channels_nb, sampling_freq);
	laWave->write(filename);

	return 0;
}
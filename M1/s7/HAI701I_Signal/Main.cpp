#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
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

void DFT(double *signal, double *partieReelle, double *partieImaginaire, int n) {

	

}

int main(int argc, char *argv[])
{
	const char* fileName = "sounds/La.wav";
	double nbSec = 6.;
	int sampling_freq = 44100;
	short channels_nb = 1;
	long int data_nb = floor(nbSec*sampling_freq);
	int freq_note = 440;
	unsigned char data8[data_nb];
	double xk;
	for (unsigned int i = 0; i < data_nb; ++i)
	{
		xk = sin(2*M_PI*freq_note*i*(1/(double)sampling_freq));
		unsigned char ucXk = doubleToUnsignedChar(xk);
		*(data8+i) = ucXk; 
	}
	Wave *laWave = new Wave(data8, data_nb, channels_nb, sampling_freq);
	laWave->write(fileName);
	return 0;
}
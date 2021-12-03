#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
#include <cstring>
#include "Wave.cpp"

double Do = 262.;
double Re = 294.;
double Mi = 330.;
double Fa = 349.;
double Sol = 392.;
double La = 440.;
double Si = 494.;

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

void real2Char(double *dataR, unsigned char *data8, int n) {

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
				data8[i] = (unsigned char)floor(127.5*(dataR[i]+1.0));
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
			signal[n] += partieReelle[k] * cos(pi2*(alpha*n)) - partieImaginaire[k] * sin(pi2*(alpha*n));
		}
	}
}

int Trouver_m(int N) {
	int m = 0;
	int M = 1;
	while (M<N) {
		m++;
		M*=2;
	}
	return m;
}


void FFT(int dir,int m, double *x,double *y)
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

void accordMagique(){
    char* filename = (char*) "AccordsMagiques.wav";
    double sample_frequency = 44100.;
    short channel_nb = 1;
    long int data_nb = (int) sample_frequency;
    unsigned char data8[data_nb];
    unsigned char fourdata8[data_nb*4];

    int m;
    m = Trouver_m(data_nb);
    std::cout<<"m: "<<m<<std::endl;
    long signal_size =(long) floor(pow(2, m));
    std::cout<<"signal size: "<<signal_size<<std::endl;
    if(signal_size > 0){
       
        double accordDo[signal_size];    //CEG
        double accordFa[signal_size];    //FAC
        double accordLam[signal_size];    //ACE
        double accordSol[signal_size];    //GBD
       
        double a4[signal_size];
        double b4[signal_size];
        double c5[signal_size];
        double d5[signal_size];
        double e5[signal_size];
        double f5[signal_size];
        double g5[signal_size];

       
        double empty_signal[signal_size];
        //double real[(int)pow(2, m)];
        double imag[signal_size];

        std::cout<<data_nb<<" "<<signal_size<<std::endl;
       
        double frac = (double) 2*M_PI/ (double)sample_frequency;
        for (int i = 0; i < data_nb; i++)
        {
            //double xk = sin(frac*nameToFrequency(0) * i);
            a4[i] = sin(frac*(double)La * i);
            b4[i] = sin(frac*Si * i);
            c5[i] = sin(frac*Do * i);
            d5[i] = sin(frac*Re * i);
            e5[i] = sin(frac*Mi * i);
            f5[i] = sin(frac*Fa * i);
            g5[i] = sin(frac*Sol * i);
        }
        for(int i = data_nb; i < signal_size; i++){
            a4[i] = 0;
            b4[i] = 0;
            c5[i] = 0;
            d5[i] = 0;
            e5[i] = 0;
            f5[i] = 0;
            g5[i] = 0;
        }

        FFT(1, m, a4, imag);
        FFT(1, m, b4, imag);
        FFT(1, m, c5, imag);
        FFT(1, m, d5, imag);
        FFT(1, m, e5, imag);
        FFT(1, m, f5, imag);
        FFT(1, m, g5, imag);
        for (int i = 0; i < signal_size; i++)
        {
            accordDo[i] = c5[i];// + e5[i] + g5[i];
            accordFa[i] = f5[i];// + a4[i] + c5[i];
            accordLam[i] = a4[i];// + c5[i] + e5[i];
            accordSol[i] = g5[i];// + b4[i] + d5[i];;
        }
        FFT(-1, m, accordDo, imag);
        FFT(-1, m, accordFa, imag);
        FFT(-1, m, accordLam, imag);
        FFT(-1, m, accordSol, imag);
        //real2charN(real, imag, data8, (int)data_nb);
        //double2realD(signal, data8, (int)data_nb);
       
        long int Fourdata_nb = (int) 4*sample_frequency;

        for (long int i = 0; i < data_nb; i++)
        {
            fourdata8[i] = doubleToUnsignedChar(a4[i]);
        }
       
       
        Wave *wav = new Wave(fourdata8, Fourdata_nb, channel_nb, sample_frequency);
        wav->write(filename);
    }
}

char* getFileName() {
	
}

void accordDo(int M, 
			  int m, 
			  double* doAcc,
			  double* miAcc, 
			  double* solAcc, 
			  double* accordDoMajeur, 
			  double* partieImaginaire,
			  int data_nb, 
			  int sampling_freq) {

	for (int i = 0; i < data_nb; ++i)
	{
		doAcc[i] = sin(2*M_PI*Do*i*(1/(double)sampling_freq));

		miAcc[i] = sin(2*M_PI*Mi*i*(1/(double)sampling_freq));

		solAcc[i] = sin(2*M_PI*Sol*i*(1/(double)sampling_freq));
	}

	for (int i = data_nb; i < M; ++i)
	{
		doAcc[i] = 0.;
		miAcc[i] = 0.;
		solAcc[i] = 0.;
		accordDoMajeur[i] = 0.;
	}

	for (int i = 0; i < data_nb; ++i)
	{
		accordDoMajeur[i] = (1./3.) * (doAcc[i] + miAcc[i] + solAcc[i]);
	}

	FFT(1, m, accordDoMajeur, partieImaginaire);
	FFT(-1, m, accordDoMajeur, partieImaginaire);

}


int main(int argc, char *argv[])
{
	const char *folder = "sounds/";
	const char *extension = ".wav";
	if (argc != 2) {
		cout << "il faut mettre un nom de fichier en parametre" << endl;
		exit(1);
	} 
	string s = string(folder) + string(argv[1]) + string(extension);
	int filenameSize = s.length();
	char filename[filenameSize+1];
	strcpy(filename, s.c_str());
	
	double nbSec = 5.;
	int sampling_freq = 44100;
	short channels_nb = 1;
	int data_nb = (int)floor(nbSec*sampling_freq);
	//int freq_note = 440;
	unsigned char data8[data_nb];
	int m = Trouver_m(data_nb);
	long int M = (int)floor(pow(2, m));

	/*int sourisVerte[35] = {La, Sol, Re, Sol, Sol, Sol, La, Re,Do,Si,La,Do,
		Si,La,Si,Do,Si,La,Sol,Re,Sol,Sol,Sol,La,Sol,Re,
	Sol,Sol,La,Sol,Sol,Sol,La,Re,Sol};*/
	double gamme[7] = {Do, Re, Mi, Fa, Sol, La, Si};

	cout << m << " " << M << " " << data_nb << endl;

	double xk;
	double partieReelle[M];
	double partieImaginaire[M];
	double doAcc[M];
	double miAcc[M];
	double solAcc[M];
	double accordDoMajeur[M];

	int gammeI = 0;
	//Gamme Chromatique
	/*for (unsigned int j = 0; j < data_nb; ++j)
	{
		partieImaginaire[j] = 0.;
		if(j%(data_nb/7) == 0 && j != 0) {
			gammeI++;
		}
		xk = sin(2*M_PI*gamme[gammeI]*j*(1/(double)sampling_freq));
		partieReelle[j] = xk;
	}

	for (int i = 0; i < M; ++i)
	{
		partieImaginaire[i] = 0.;
		//partieReelle[i] = 0.;
	}*/

	//Accord Do Majeur
	for (int i = 0; i < data_nb; ++i)
	{
		doAcc[i] = sin(2*M_PI*Do*i*(1/(double)sampling_freq));

		miAcc[i] = sin(2*M_PI*Mi*i*(1/(double)sampling_freq));

		solAcc[i] = sin(2*M_PI*Sol*i*(1/(double)sampling_freq));
	}

	for (int i = data_nb; i < M; ++i)
	{
		doAcc[i] = 0.;
		miAcc[i] = 0.;
		solAcc[i] = 0.;
	}

	/*FFT(1, m, doAcc, partieImagjeur[M];inaire);
	FFT(1, m, miAcc, partieImaginaire);
	FFT(1, m, solAcc, partieImaginaire);*/

	for (int i = 0; i < data_nb; ++i)
	{
		accordDoMajeur[i] = (1./3.) * (doAcc[i] + miAcc[i] + solAcc[i]);
	}

	for (int i = data_nb; i < M; ++i)
	{
		accordDoMajeur[i] = 0.;
	}

	//FFT(-1, m, accordDoMajeur, partieImaginaire);
	
	/*FFT(1, m, partieReelle, partieImaginaire);

	FFT(-1, m, partieReelle, partieImaginaire);*/

	real2Char(accordDoMajeur, data8, data_nb);

	Wave *laWave = new Wave(data8, data_nb, channels_nb, sampling_freq);
	laWave->write(filename);

	return 0;
}
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include "Wave.cpp"

int main(int argc, char *argv[])
{
	Wave *wave = new Wave();
	wave->read("sounds/Whistle.wav");
	Wave *LaWave = new Wave(wave->data8, wave->data_size, 2, 440);
	LaWave->write("sounds/La.wav");
	return 0;
}
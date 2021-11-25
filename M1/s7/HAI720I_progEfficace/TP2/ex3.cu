#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "image-pnm.h"
#include <string>

using namespace std;

int B = 1;
void blur_pbm(Image<float> &source, Image<float> &cible) {

	size_t width = source.width();
	size_t height = source.height();

	float* data = source.getData();
	float* out = cible.getData();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int minX = i - B < 0 ? 0 : i - B;
			int maxX = i + B > width-1 ? width : i + B;

			int minY = j - B < 0 ? 0 : j - B;
			int maxY = j + B > height-1 ? height : j + B;

			int color = 0;
			int count = 0;

			for (int k = minX; k < maxX; k++)
			{
				for (int l = minY; l < maxY; l++)
				{
					color += data[k*width+l];
					cout << k << " " << l << endl;
					count++;  
				}
			}
			cout << "count : " << count << endl;
			color = (float)(color/count);
			cout << color << endl;
			out[i*width+j] = color;
		}
	}
}


int main() {

	Image<float> in = Image<float>("P2", 512, 512);
	in.read("./image-grey.pgm");

	Image<float> out = Image<float>("P2", in.width(), in.height());

	blur_pbm(in, out);
	out.write("./blurred.pgm");

	return 0;

}


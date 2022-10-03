#include "CImg.h"
#include "string.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace cimg_library;
using namespace std;

// Definition des couleurs de trace (rouge, vert, bleu)
const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };

int Intersection( double L[3], int Dx, int Dy, int x_inter[2], int y_inter[2] )
{
	double a=L[0], b=L[1], c=L[2] ;
	double x[4], y[4] ;
	int nb_points_ok = 0, n ;
	x[0] = 0 ;
	x[1] = Dx-1 ;
	y[2] = 0 ;
	y[3] = Dy-1 ;
	
	if(fabs(L[0])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		b = -L[1]/L[0] ;
		c = -L[2]/L[0] ;
		x[2] = b * y[2] + c ;
		x[3] = b * y[3] + c ;
	}
	else 
	{
		x[2] = -Dx ;
		x[3] = -Dx ;
	}

	if(fabs(L[1])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		a = -L[0]/L[1] ;
		c = -L[2]/L[1] ;
		y[0] = a * x[0] + c ;
		y[1] = a * x[1] + c ;
	}
	else 
	{
		y[0] = -Dy ;
		y[1] = -Dy ;
	}
	
	for(n=0 ; n<4 ; n++)
	{
		if( (x[n]>=0.0) && (y[n]>=0.0) && (x[n]<=Dx) && (y[n]<=Dy) && (nb_points_ok<2) )
		{
			x_inter[nb_points_ok] = (int)floor(x[n]+0.5) ;
			y_inter[nb_points_ok] = (int)floor(y[n]+0.5) ;
			nb_points_ok ++ ;
		}
	}
	
	if(nb_points_ok==2) return 1 ; 
	else return 0 ;	
}

struct Point {
	int i;
	int j;
};

struct Segment {
  	Point *start;
  	Point *end;

  	void drawLine(CImg<unsigned char> &img, CImgDisplay &imgDisp) {
  		int x_inter[2], y_inter[2] ;
  		double L[3];

		L[0] = (double)( end->j - start->j ) ;
		L[1] = (double)( start->i - end->i ) ;
		L[2] = (double)end->i*(double)start->j - (double)start->i*(double)end->j ;		
		
		int n = Intersection(L, img.width(), img.height(), x_inter, y_inter ) ;
		img.draw_line(x_inter[0],y_inter[0],x_inter[1],y_inter[1],red).display(imgDisp);
  	}
};

int main(int argc, char *argv[])
{
	int nbPoints = 2, n = 0;
	vector<Point*> points;
	vector<Segment*> segments;

	if(argc != 2) {
		printf("\nCe programme a 1 argument d'appel qui est une image\n") ;
		printf("Il faut le lancer de la forme ./main ImageDroite.***\n") ;
		exit(0);
	}

	CImg<unsigned char> img(argv[1]);

	// Creation des objets d'affichage
	CImgDisplay imgDisp(img,"Image");

	while (!imgDisp.is_closed()) 
	{
		imgDisp.wait();
		if (imgDisp.button() && imgDisp.mouse_y()>=0) {
			Point *p = new Point(); {p->i = imgDisp.mouse_x(); p->j = imgDisp.mouse_y();}
			points.push_back(p);
			n++;
			cout << "i : " << p->i << " ; j : " << p->j << endl; 
			if(n == nbPoints) {
				Segment *s = new Segment(); 
				{ 
					s->start = points[points.size()-2]; 
					s->end = points[points.size()-1]; 
				}
				segments.push_back(s);
				s->drawLine(img, imgDisp);
				n = 0;
			}
			cout << "n : " << n << endl; 
		}
	}

}
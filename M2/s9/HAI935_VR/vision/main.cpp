#include "CImg.h"
#include "string.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace cimg_library;
using namespace std;

// Definition des couleurs de trace (rouge, vert, bleu)
const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };

struct Point {
	int i;
	int j;
};

struct Segment {
  	Point *start;
  	Point *end;

  	void drawLine(CImg<unsigned char> img) {
  		int x_inter[2], y_inter[2] ;
		img.draw_line(start->i,start->j,end->i,end->j,green);

		L[0] = (double)( end->j - end->i ) ;
		L[1] = (double)( start->i - start->j ) ;
		L[2] = (double)xd[1]*(double)yd[0] - (double)xd[0]*(double)yd[1] ;		
				
		n = Intersection(L, img.width(), img.height(), x_inter, y_inter ) ;
		img.draw_line(x_inter[0],y_inter[0],x_inter[1],y_inter[1],red).display(Droite_disp);
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
			if(n < nbPoints) {
				Point *p = new Point(); {p->i = imgDisp.mouse_y(); p->j = imgDisp.mouse_y();}
				points.push_back(p);
				n++;
				cout << "i : " << p->i << " ; j : " << p->j << endl; 
			} else if(n == nbPoints) {
				Segment *s = new Segment(); 
				{ 
					s->start = points[0]; 
					s->end = points[1]; 
				}
				segments.push_back(s);
				s.drawLine(img);
			}
			else n = 0;
		}
	}

}
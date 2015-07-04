#ifndef NODE
#define NODE
   
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "qimage.h"
#include "qmessagebox.h"
#include <vector>


using namespace std;
using namespace cv;


class GNode
{
public:
	CvPoint point;
	double difference;
	double length;

	GNode* parent;

	bool inmask;
	bool visited;
	bool expanded;
public:
	GNode(){
		point.x = point.y = 0;
		difference = 0;
		length = 0;
	    parent = NULL;
		inmask = false;
		visited = false;
		expanded = false;
	}
	void setParent(GNode* p){
	    parent = p;
	}

	void set(IplImage* src, IplImage* target, int x, int y, bool in = false)
	{
		point.x = x, point.y = y;
		inmask = in;
		
		if(src != NULL)
		{
            difference = 0;
			difference += abs(CV_IMAGE_ELEM(target, char, x, 3*y) - CV_IMAGE_ELEM(src, char, x, 3*y));
			difference += abs(CV_IMAGE_ELEM(target, char, x, 3*y+1) - CV_IMAGE_ELEM(src, char, x, 3*y+1));
			difference += abs(CV_IMAGE_ELEM(target, char, x, 3*y+2) - CV_IMAGE_ELEM(src, char, x, 3*y+2));	
			difference /= 10;
		}
		else
		{
		    difference = DBL_MAX;
		}
	}
	bool operator == (GNode node){
		return this->length == node.length; //length越大，优先度越低
	}
	bool operator > (GNode node){
		return this->length < node.length; //
	}
	bool operator >= (GNode node){
		return this->length <= node.length; //
	}
	bool operator < (GNode node){
		return this->length > node.length; //
	}
	bool operator <= (GNode node){
		return this->length >= node.length; //
	}

};

#endif
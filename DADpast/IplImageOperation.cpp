#include "IplImageOperation.h"
#include <qdebug.h>

void SetPixel1C(IplImage* img, int x, int y, int r){
	if (img == NULL || img->nChannels != 1){
		return;
	}if (x <0 || y <0 || x>img->width-1 || y>img->height-1 )
		return;
	CV_IMAGE_ELEM(img, char, y, x) = r;
}
   
void SetPixel3C(IplImage* img, int x, int y, int b, int r, int g){
	if (img == NULL || img->nChannels != 3){
		return;
	}if (x <0 || y <0 || x>img->width-1 || y>img->height-1 )
		return;
	CV_IMAGE_ELEM(img, char, y, 3*x) = b;
	CV_IMAGE_ELEM(img, char, y, 3*x+1) = r;
	CV_IMAGE_ELEM(img, char, y, 3*x+2) = g; 
}

void SetSameColor(IplImage* img, int s){
	if (img->nChannels == 3){
		for (int i=0; i<img->height; i++)
			for (int j=0; j<img->width; j++){
				CV_IMAGE_ELEM(img, char, i, 3*j) = s;
				CV_IMAGE_ELEM(img, char, i, 3*j+1) = s;
				CV_IMAGE_ELEM(img, char, i, 3*j+2) = s; 
			}
	}
	else if (img->nChannels == 1){
		for (int i=0; i<img->height; i++)
			for (int j=0; j<img->width; j++)
				CV_IMAGE_ELEM(img, char, i, j) = s;
	}
}


IplImage* cvGet4Channels(IplImage* img, IplImage* mattee){
	if (img == NULL || mattee == NULL || img->height != mattee->height || img->width != mattee->width)
		return NULL;
	if (img->nChannels != 3 || mattee->nChannels != 1)
		return NULL;
	IplImage* res = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 4);
	for (int i=0; i<img->height; i++){
		for (int j=0; j<img->width; j++){
			CV_IMAGE_ELEM(res, char, i, 4*j) = CV_IMAGE_ELEM(img, char, i, 3*j);
			CV_IMAGE_ELEM(res, char, i, 4*j+1) = CV_IMAGE_ELEM(img, char, i, 3*j+1);
			CV_IMAGE_ELEM(res, char, i, 4*j+2) = CV_IMAGE_ELEM(img, char, i, 3*j+2);
			CV_IMAGE_ELEM(res, unsigned char, i, 4*j+3) = CV_IMAGE_ELEM(mattee, unsigned char, i, j);
		}
	}
	return res;
}

IplImage* cvCombineImage(IplImage* back, IplImage* src, int x, int y){
	if (src == NULL || back == NULL)
		return NULL;
	int width = back->width;
	int height = back->height;

	IplImage* res = cvCloneImage(back);
	int i, j;
	i = (y>0)? y:0;
	
	int x_max = src->width+x;
	int y_max = src->height+y;
	for ( ; i<height && i<y_max; i++){
		j = (x>0)? x:0;
		for ( ;j<width && j<x_max; j++){
			float s = CV_IMAGE_ELEM(src, unsigned char, i-y, 4*(j-x)+3);
			s = s/255;
			CV_IMAGE_ELEM(res, char, i, 3*j) = (1-s)*CV_IMAGE_ELEM(back, unsigned char, i, 3*j) + s*CV_IMAGE_ELEM(src, unsigned char, i-y, 4*(j-x));
			CV_IMAGE_ELEM(res, char, i, 3*j+1) = (1-s)*CV_IMAGE_ELEM(back, unsigned char, i, 3*j+1) + s*CV_IMAGE_ELEM(src, unsigned char, i-y, 4*(j-x)+1);
			CV_IMAGE_ELEM(res, char, i, 3*j+2) = (1-s)*CV_IMAGE_ELEM(back, unsigned char, i, 3*j+2) + s*CV_IMAGE_ELEM(src, unsigned char, i-y, 4*(j-x)+2);
		}
	}
	return res;
}

IplImage* cvScaleImage(IplImage* img, float s){
	int height = img->height*s;
	int width = img->width*s;
	IplImage* res = cvCreateImage(cvSize(width, height), img->depth, img->nChannels);
	if (res->nChannels == 1){
		for (int i=0; i<height; i++){
			for (int j=0; j<width; j++){
				int x = j/s, y = i/s;
				CV_IMAGE_ELEM(res, char, i, j) = CV_IMAGE_ELEM(img, char, y, x);
			}
		}
	}else if (res->nChannels == 3){
		for (int i=0; i<height; i++){
			for (int j=0; j<width; j++){
				int x = j/s, y = i/s;
				CV_IMAGE_ELEM(res, char, i, 3*j) = CV_IMAGE_ELEM(img, char, y, 3*x);
				CV_IMAGE_ELEM(res, char, i, 3*j+1) = CV_IMAGE_ELEM(img, char, y, 3*x+1);
				CV_IMAGE_ELEM(res, char, i, 3*j+2) = CV_IMAGE_ELEM(img, char, y, 3*x+2);
			}
		}
	}else if (res->nChannels == 4){
		for (int i=0; i<height; i++){
			for (int j=0; j<width; j++){
				int x = j/s, y = i/s;
				CV_IMAGE_ELEM(res, char, i, 4*j) = CV_IMAGE_ELEM(img, char, y, 4*x);
				CV_IMAGE_ELEM(res, char, i, 4*j+1) = CV_IMAGE_ELEM(img, char, y, 4*x+1);
				CV_IMAGE_ELEM(res, char, i, 4*j+2) = CV_IMAGE_ELEM(img, char, y, 4*x+2);
				CV_IMAGE_ELEM(res, char, i, 4*j+3) = CV_IMAGE_ELEM(img, char, y, 4*x+3);
			}
		}
	}
	return res;
}
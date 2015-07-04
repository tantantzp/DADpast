#ifndef IPLIMAGEOPERATION_H
#define IPLIMAGEOPERATOPN_H
   
#include <opencv2\opencv.hpp>

void SetPixel1C(IplImage* img, int x, int y, int r);

void SetPixel3C(IplImage* img, int x, int y, int b, int r, int g);

void SetSameColor(IplImage* img, int s);

IplImage* cvCombineImage(IplImage* back, IplImage* src, int x, int y);

IplImage* cvGet4Channels(IplImage* img, IplImage* mattee);

IplImage* cvScaleImage(IplImage* img, float s);
#endif
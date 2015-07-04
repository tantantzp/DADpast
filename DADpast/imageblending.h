#ifndef IMAGEBLENDING_H
#define IMAGEBLENDING_H
  
#include "MVC/MVCoordinate.h"
#include <opencv/cv.h>
#include <QMap>
using namespace cv;
using namespace MVC_CLONE;

class ImageBlending : public QObject
{
	Q_OBJECT

public:
	ImageBlending(QObject *parent);
	ImageBlending();
	ImageBlending(const QImage& src, const QImage& tgt, const QVector<QPoint>& _patchPath);
	~ImageBlending();

// 	inline double sqrDist(const cv::Point& p1, const cv::Point& p2)
// 	{
// 		return (p1.x - p2.x) * (p1.x - p2.x) + 
// 			   (p1.y - p2.y) * (p1.y - p2.y);
// 	}

	void Init(const QImage& src, const QImage& tgt, const QVector<QPoint>& _patchPath);

	void MVCBlending(const QPoint&, QMap<int, Vec3d>&, Mat&);
	void MVCBlending(const QPoint&);
	/*double HybridGradBlending(const Mat&, double, const QPoint&);*/

	void ReleaseMemory();

	/*double SumOFDiff();*/

private:
	void RegionTriangulate();
	void CalBoundaryDiff();

	void ConstructMatrixLB(CvSparseMat *, CvMat*);
	void ConstructMatrixBlending(CvSparseMat *, CvMat*, int);
	
	/*void CalculateHybridGrad();
	void CalculateHybridDiv();*/

	bool GenBlendMap();
	/*void GenHybridGrad();*/

	inline double sqrDist(double d1, double d2)
	{
		return d1 * d1 + d2 * d2;
	}

public:
	QImage blendTrimap;
	QImage blendingResult;
	QImage blendingResultTgt;
	QImage blendingHybridRst;

	QImage salientTrimap;

	QImage targetImage;
	QImage sourceImage;

	bool isHybridGrad;
	bool isTriangulate;

	QPoint pastePos;
	
	QMap<int, QVector<double> > divHybrid;
	QMap<int, QVector<QPointF> > gradHybrid;

	int indexN;
	CvMat* matX;	
	QMap<int, int> bandMap;
	QMap<int, int> blendMap;
	int blendN;

	QVector<QVector<double> >matRgb;

	MvcPerporties MVCoord;
	QVector<QPoint> MVCBoundary;
	QVector<QPoint> MVCVertex;
	QVector<double> boundaryDiff;
	QVector<double> membrane;

	QVector<int> TriangleList;
	QVector<BCofRegionPoint> TriRegionPoint;

};

#endif // IMAGEBLENDING_H

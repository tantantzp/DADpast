#pragma once
#include <QPoint>
#include <QVector>
#include <QImage>
namespace MVC_CLONE
{
	struct MvcPerporties
	{
		int BoundarySize;
		int PointSize;
		double *mvcArray;
	};

	struct BCofRegionPoint
	{
		int seq;
		double v1;
		double v2;
		double v3;
	};


	double CalculateWeights(QPointF p, QVector<QPoint> *boundary, double *weights);

	void findTriangle(QVector <BCofRegionPoint> &RegionPoint, QVector<QPoint> *vertex, QVector<int> *triList, int width);

	void barycenter(BCofRegionPoint &tmp, int x, int y, QPoint v1, QPoint v2, QPoint v3);

	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates);

	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates, QImage& mask);

	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates, QVector<int>& mask);

}




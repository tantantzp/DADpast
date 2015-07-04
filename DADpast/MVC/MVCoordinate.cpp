#include "stdafx.h"
#include "MVCoordinate.h"
#include "Vector3f.h"

namespace MVC_CLONE
{
	const double EPSILON = 1e-6;

	double CalculateWeights(QPointF p, QVector<QPoint> *boundary, double *weights)
	{
		double sum = 0.;
		int size = boundary->size() - 1;
		CVector3f pointX;
		CVector3f p0, p1, p2;
		CVector3f v1, v2;

		pointX.x = p.x() + EPSILON;
		pointX.y = p.y() + EPSILON;
		pointX.z = .0;

		memset(weights, 0, sizeof(double) * size);

		for (int i = 0; i < size; i++)
		{
			p0.x = boundary->at((i - 1 + size) % size).x();
			p0.y = boundary->at((i - 1 + size) % size).y();
			p0.z = .0;

			p1.x = boundary->at(i).x();
			p1.y = boundary->at(i).y();
			p1.z = 0.00001;

			p2.x = boundary->at((i + 1 + size) % size).x();
			p2.y = boundary->at((i + 1 + size) % size).y();
			p2.z = .0;

			v1 = p0 - pointX;
			v2 = p1 - pointX;

			double angle1 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle1 = -angle1;

			v1 = v2;
			v2 = p2 - pointX;

			double angle2 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle2 = -angle2;

			weights[i] = (tan(angle1 / 2.) + tan(angle2 / 2.)) / (Distance(p1, pointX) + 0.001);
			sum += weights[i];
		}

		return sum;
	}

	void findTriangle(QVector <BCofRegionPoint> &RegionPoint, QVector<QPoint> *vertex, QVector<int> *triList, int width)
	{
		int left, right, up, bottom;
		QPoint vertex1,vertex2,vertex3;
		BCofRegionPoint tmp;

		// QFile file("Vertex.txt");
		// if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		//	 return;
		// QTextStream out(&file);	

		for(int i = 0; i < triList->size() / 3; ++i)
		{
			QPainterPath triangle;
			vertex1 = vertex->at(triList->at(i * 3));
			vertex2 = vertex->at(triList->at(i * 3 + 1));
			vertex3 = vertex->at(triList->at(i * 3 + 2));

			//out << i <<":"<<triList->at(i*3)<<" "<<triList->at(i*3 + 1)<<" "<<triList->at(i*3 + 2)<<"\n";
			//file.flush();

			triangle.moveTo(vertex1);
			triangle.lineTo(vertex2);
			triangle.lineTo(vertex3);
			//triangle.lineTo(vertex1);
			
			tmp.seq = i;
			tmp.v1 = 1.;
			tmp.v2 = 0.;
			tmp.v3 = 0.;
			RegionPoint.push_back(tmp);
			
			tmp.v1 = 0.;
			tmp.v2 = 1.;
			tmp.v3 = 0.;
			RegionPoint.push_back(tmp);
			
			tmp.v1 = 0.;
			tmp.v2 = 0.;
			tmp.v3 = 1.;
			RegionPoint.push_back(tmp);

			left = MIN(MIN(vertex1.x(),vertex2.x()),vertex3.x());
			right = MAX(MAX(vertex1.x(),vertex2.x()),vertex3.x());
			up = MIN(MIN(vertex1.y(),vertex2.y()),vertex3.y());
			bottom = MAX(MAX(vertex1.y(),vertex2.y()),vertex3.y());

			cv::Mat rp(bottom-up+1,right-left+1,CV_8UC3);
			rp.setTo(0);
			std::ofstream of("qt.txt");
			int count = 0;
			for(int n = up; n <= bottom; ++n)
			{
				for(int m = left; m <= right; ++m)
				{
					if(triangle.contains(QPoint(m,n)))
					{
						tmp.seq = i;
						barycenter(tmp, m, n, vertex1,vertex2,vertex3);
						RegionPoint.push_back(tmp);
						rp.at<cv::Vec3b>(n-up,m-left) = cv::Vec3b(255,255,255);
						of<<m<<" "<<n<<";";
						count++;
					}
				}
			}
			of<<std::endl<<count;
			of.close();
			cv::imwrite("rp.png",rp);
		}

		//	file.close();

	}

	void barycenter(BCofRegionPoint &tmp, int x, int y, QPoint v1, QPoint v2, QPoint v3)
	{
		CVector3f a1(v1.x()-v2.x(),v1.y()-v2.y(),0);	//v1-v2
		CVector3f a2(v3.x()-v2.x(),v3.y()-v2.y(),0);	//v3-v2

		double s = Magnitude(Cross(a1,a2));

		a1.x = v1.x()-x;
		a1.y = v1.y()-y;	
		a2.x = v2.x()-x;
		a2.y = v2.y()-y;

		double s1 = Magnitude(Cross(a1,a2));
		tmp.v3 = s1 / s;

		a2.x = v3.x()-x;
		a2.y = v3.y()-y;

		s1 = Magnitude(Cross(a1,a2));
		tmp.v2 = s1 / s;

		a1.x = v2.x()-x;
		a1.y = v2.y()-y;


		s1 = Magnitude(Cross(a1,a2));
		tmp.v1 = s1 / s;

	/*
		SuperMatrix A;
		double rhs[3] = {1.0,2.0,3.0};
		dCreate_Dense_Matrix(&A, 3, 1, rhs, 1, SLU_DN, SLU_D, SLU_GE);

		int a =0;

	*/
	}
	
	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates)
	{
		int BoundarySize = mvc->BoundarySize;
		double *mvcArray = mvc->mvcArray;

		double *weightArray = new double[BoundarySize];

		int vertexSize = Coordinates->size();

		for (int i = 0; i < vertexSize; ++i)
		{
			double sum = CalculateWeights(QPointF(Coordinates->at(i)), boundary, weightArray);
			for (int j = 0; j < BoundarySize; ++j)
				*(mvcArray + i * BoundarySize + j) = weightArray[j] / sum;
		}

		delete[] weightArray;
	}



	//////////////////////////////////////////////////////////////////////////

	double CalculateWeightsMask(QPointF p, QVector<QPoint> *boundary, double *weights, QImage& mask)
	{
		double sum = 0.;
		int size = boundary->size() - 1;
		CVector3f pointX;
		CVector3f p0, p1, p2;
		CVector3f v1, v2;

		pointX.x = p.x() + EPSILON;
		pointX.y = p.y() + EPSILON;
		pointX.z = .0;

		memset(weights, 0, sizeof(double) * size);

		for (int i = 0; i < size; i++)
		{
			QPoint pt = boundary->at(i);

			if (qGray(mask.pixel(pt)) < 200) // if the boundary point is not marked
			{
				weights[i] = 0;
				continue;
			}			

			p0.x = boundary->at((i - 1 + size) % size).x();
			p0.y = boundary->at((i - 1 + size) % size).y();
			p0.z = .0;

			p1.x = boundary->at(i).x();
			p1.y = boundary->at(i).y();
			p1.z = 0.00001;

			p2.x = boundary->at((i + 1 + size) % size).x();
			p2.y = boundary->at((i + 1 + size) % size).y();
			p2.z = .0;

			v1 = p0 - pointX;
			v2 = p1 - pointX;

			double angle1 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle1 = -angle1;

			v1 = v2;
			v2 = p2 - pointX;

			double angle2 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle2 = -angle2;

			weights[i] = (tan(angle1 / 2.) + tan(angle2 / 2.)) / (Distance(p1, pointX) + 0.001);
			sum += weights[i];
		}

		return sum;
	}

	double CalculateWeightsVec(QPointF p, QVector<QPoint> *boundary, double *weights, QVector<int>& mask)
	{
		double sum = 0.;
		int size = boundary->size() - 1;
		CVector3f pointX;
		CVector3f p0, p1, p2;
		CVector3f v1, v2;

		pointX.x = p.x() + EPSILON;
		pointX.y = p.y() + EPSILON;
		pointX.z = .0;

		memset(weights, 0, sizeof(double) * size);

		for (int n = 0; n < mask.size(); ++n)
		{
			int i = mask[n];

			QPoint pt = boundary->at(i);				

			p0.x = boundary->at((i - 1 + size) % size).x();
			p0.y = boundary->at((i - 1 + size) % size).y();
			p0.z = .0;

			p1.x = boundary->at(i).x();
			p1.y = boundary->at(i).y();
			p1.z = 0.00001;

			p2.x = boundary->at((i + 1 + size) % size).x();
			p2.y = boundary->at((i + 1 + size) % size).y();
			p2.z = .0;

			v1 = p0 - pointX;
			v2 = p1 - pointX;

			double angle1 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle1 = -angle1;

			v1 = v2;
			v2 = p2 - pointX;

			double angle2 = AngleBetweenVectors(v1, v2);
			if (v1.x * v2.y - v1.y * v2.x > 0)
				angle2 = -angle2;

			weights[i] = (tan(angle1 / 2.) + tan(angle2 / 2.)) / (Distance(p1, pointX) + 0.001);
			sum += weights[i];

		}

// 		for (int i = 0; i < size; i++)
// 		{
// 			QPoint pt = boundary->at(i);
// 
// 			if (!mask.contains(i)) // if the boundary point is not marked
// 			{
// 				weights[i] = 0;
// 				continue;
// 			}			
// 
// 			p0.x = boundary->at((i - 1 + size) % size).x();
// 			p0.y = boundary->at((i - 1 + size) % size).y();
// 			p0.z = .0;
// 
// 			p1.x = boundary->at(i).x();
// 			p1.y = boundary->at(i).y();
// 			p1.z = 0.00001;
// 
// 			p2.x = boundary->at((i + 1 + size) % size).x();
// 			p2.y = boundary->at((i + 1 + size) % size).y();
// 			p2.z = .0;
// 
// 			v1 = p0 - pointX;
// 			v2 = p1 - pointX;
// 
// 			double angle1 = AngleBetweenVectors(v1, v2);
// 			if (v1.x * v2.y - v1.y * v2.x > 0)
// 				angle1 = -angle1;
// 
// 			v1 = v2;
// 			v2 = p2 - pointX;
// 
// 			double angle2 = AngleBetweenVectors(v1, v2);
// 			if (v1.x * v2.y - v1.y * v2.x > 0)
// 				angle2 = -angle2;
// 
// 			weights[i] = (tan(angle1 / 2.) + tan(angle2 / 2.)) / (Distance(p1, pointX) + 0.001);
// 			sum += weights[i];
// 		}

		return sum;
	}

	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates, QImage& mask)
	{
		int BoundarySize = mvc->BoundarySize;
		double *mvcArray = mvc->mvcArray;

		double *weightArray = new double[BoundarySize];

		int vertexSize = Coordinates->size();

		for (int i = 0; i < vertexSize; ++i)
		{
			double sum = CalculateWeightsMask(QPointF(Coordinates->at(i)), boundary, weightArray, mask);
			for (int j = 0; j < BoundarySize; ++j)
				*(mvcArray + i * BoundarySize + j) = weightArray[j] / sum;			
		}

		delete[] weightArray;
	}

	void CalculateMVCoord(QVector<QPoint> *boundary, MvcPerporties *mvc, QVector<QPoint> *Coordinates, QVector<int>& mask)
	{
		int BoundarySize = mvc->BoundarySize;
		double *mvcArray = mvc->mvcArray;

		double *weightArray = new double[BoundarySize];

		int vertexSize = Coordinates->size();

		for (int i = 0; i < vertexSize; ++i)
		{
			double sum = CalculateWeightsVec(QPointF(Coordinates->at(i)), boundary, weightArray, mask);
			for (int j = 0; j < BoundarySize; ++j)
				*(mvcArray + i * BoundarySize + j) = weightArray[j] / sum;			
		}

		delete[] weightArray;
	}

	
}


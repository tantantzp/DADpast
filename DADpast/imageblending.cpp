
#include "imageblending.h"
#include "MVC/Triangle/triangulate.h"
#include <QColor>
#include <QDebug>

#include <opencv/cv.h>
ImageBlending::ImageBlending(QObject *parent)
	: QObject(parent)
{
}
 
ImageBlending::ImageBlending()
{
	MVCoord.mvcArray = NULL;
	matX = NULL;

	isHybridGrad = false;
	isTriangulate = false;
	pastePos = QPoint(0, 0);

}

// this is invalid anymore
ImageBlending::ImageBlending(const QImage& _src, const QImage& _tgt, const QVector<QPoint>& _patchPath)
{
	sourceImage = _src;
	targetImage = _tgt;

	blendTrimap = QImage(_src);
	blendTrimap.fill(0);	
	
	MVCBoundary = _patchPath;
	MVCVertex = _patchPath;
	MVCoord.mvcArray = NULL;
	
	blendingResult = _src;
	blendingResultTgt = _tgt;
	pastePos = QPoint(0, 0);

	matX = NULL;

	isHybridGrad = false;
	isTriangulate = false;

}

void ImageBlending::Init(const QImage& _src, const QImage& _tgt, const QVector<QPoint>& _patchPath)
{
	sourceImage = _src;
	targetImage = _tgt;

	blendTrimap = QImage(_src);
	blendTrimap.fill(0);

	salientTrimap = _src;
	salientTrimap.fill(0);

	MVCBoundary = _patchPath;
	MVCVertex = _patchPath;

	blendingResult = _src;
	blendingResultTgt = _tgt;
	pastePos = QPoint(0, 0);	

	isHybridGrad = false;
	isTriangulate = false;

	divHybrid.clear();
	gradHybrid.clear();

	if (matX) cvReleaseMat(&matX);
	if (MVCoord.mvcArray) 
	{
		delete MVCoord.mvcArray;
		MVCoord.mvcArray = NULL;
	}

}

ImageBlending::~ImageBlending()
{
	if (matX) cvReleaseMat(&matX);
	if (MVCoord.mvcArray) 
	{
		delete MVCoord.mvcArray;
		MVCoord.mvcArray = NULL;
	}

	MVCBoundary.clear();
	MVCVertex.clear();
	TriRegionPoint.clear();
	divHybrid.clear();
	gradHybrid.clear();

}

void ImageBlending::ReleaseMemory()
{
	if (matX) cvReleaseMat(&matX);
	if (MVCoord.mvcArray) 
	{
		delete MVCoord.mvcArray;
		MVCoord.mvcArray = NULL;
	}

	MVCBoundary.clear();
	MVCVertex.clear();
	TriRegionPoint.clear();
	divHybrid.clear();
	gradHybrid.clear();
}

//////////////////////////////////////////////////////////////////////////
// MVC cloning

void ImageBlending::MVCBlending(const QPoint& pt, QMap<int, Vec3d>& lapOffset, Mat& mask)
{
	pastePos = pt;

	if (!isTriangulate)
	{
		RegionTriangulate();
		isTriangulate = true;
	}

	CalBoundaryDiff();

	membrane.clear();
	int RegionSize = MVCoord.PointSize;
	int BoundarySize = MVCoord.BoundarySize;

	for (int i = 0; i < RegionSize; ++i)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		double *temp = MVCoord.mvcArray + i * BoundarySize;
		for (int k = 0; k < BoundarySize; ++k)
		{
			double t = *(temp + k);
			r += t * boundaryDiff.at(3 * k);
			g += t * boundaryDiff.at(3 * k + 1);
			b += t * boundaryDiff.at(3 * k + 2);
		}

		membrane.append(r);
		membrane.append(g);
		membrane.append(b);
	}

	BCofRegionPoint pixel;
	QColor resultColor;

	for (int i = 0; i < TriRegionPoint.size(); ++i)
	{
		pixel = TriRegionPoint[i];

		int seq1 = TriangleList[pixel.seq * 3];
		int seq2 = TriangleList[pixel.seq * 3 + 1];
		int seq3 = TriangleList[pixel.seq * 3 + 2];

		int x = pixel.v1 * MVCVertex[seq1].x() + 
			pixel.v2 * MVCVertex[seq2].x() + 
			pixel.v3 * MVCVertex[seq3].x() + 0.5;

		int y = pixel.v1 * MVCVertex[seq1].y() + 
			pixel.v2 * MVCVertex[seq2].y() + 
			pixel.v3 * MVCVertex[seq3].y() + 0.5;

		if (!targetImage.rect().contains(pastePos + QPoint(x, y)))
			continue;

		double r = pixel.v1 * membrane[seq1 * 3] + pixel.v2 * membrane[seq2 * 3] + 
			pixel.v3 * membrane[seq3 * 3];

		double g = pixel.v1 * membrane[seq1 * 3 + 1] + pixel.v2 * membrane[seq2 * 3 + 1] + 
			pixel.v3 * membrane[seq3 * 3 + 1];

		double b = pixel.v1 * membrane[seq1 * 3 + 2] + pixel.v2 * membrane[seq2 * 3 + 2] + 
			pixel.v3 * membrane[seq3 * 3 + 2];

		resultColor.setRgb(sourceImage.pixel(x, y));

		resultColor.setRedF(MAX(MIN((resultColor.redF() + r), 1), 0));
		resultColor.setGreenF(MAX(MIN((resultColor.greenF() + g), 1), 0));
		resultColor.setBlueF(MAX(MIN((resultColor.blueF() + b), 1), 0));

		blendingResult.setPixel(x, y, resultColor.rgb());
		blendingResultTgt.setPixel(pastePos + QPoint(x, y), resultColor.rgb());

		Vec3d vec3d_;
		vec3d_[0] = r;
		vec3d_[1] = g;
		vec3d_[2] = b;
 
 		lapOffset.insert(x + y * sourceImage.width(), vec3d_);
		mask.at<uchar>(y, x) = 255;

	}

}

void ImageBlending::MVCBlending(const QPoint& pt)
{
	pastePos = pt;

	if (!isTriangulate)
	{
		RegionTriangulate();
		isTriangulate = true;
	}

	CalBoundaryDiff();

	membrane.clear();
	int RegionSize = MVCoord.PointSize;
	int BoundarySize = MVCoord.BoundarySize;

	for (int i = 0; i < RegionSize; ++i)
	{
		double r = 0;
		double g = 0;
		double b = 0;

		double *temp = MVCoord.mvcArray + i * BoundarySize;
		for (int k = 0; k < BoundarySize; ++k)
		{
			double t = *(temp + k);
			r += t * boundaryDiff.at(3 * k);
			g += t * boundaryDiff.at(3 * k + 1);
			b += t * boundaryDiff.at(3 * k + 2);
		}

		membrane.append(r);
		membrane.append(g);
		membrane.append(b);
	}

	BCofRegionPoint pixel;
	QColor resultColor;
	
	for (int i = 0; i < TriRegionPoint.size(); ++i)
	{
		pixel = TriRegionPoint[i];

		int seq1 = TriangleList[pixel.seq * 3];
		int seq2 = TriangleList[pixel.seq * 3 + 1];
		int seq3 = TriangleList[pixel.seq * 3 + 2];

		int x = pixel.v1 * MVCVertex[seq1].x() + 
			pixel.v2 * MVCVertex[seq2].x() + 
			pixel.v3 * MVCVertex[seq3].x() + 0.5;

		int y = pixel.v1 * MVCVertex[seq1].y() + 
			pixel.v2 * MVCVertex[seq2].y() + 
			pixel.v3 * MVCVertex[seq3].y() + 0.5;

		if (!targetImage.rect().contains(pastePos + QPoint(x, y)))
			continue;

		double r = pixel.v1 * membrane[seq1 * 3] + pixel.v2 * membrane[seq2 * 3] + 
			pixel.v3 * membrane[seq3 * 3];

		double g = pixel.v1 * membrane[seq1 * 3 + 1] + pixel.v2 * membrane[seq2 * 3 + 1] + 
			pixel.v3 * membrane[seq3 * 3 + 1];

		double b = pixel.v1 * membrane[seq1 * 3 + 2] + pixel.v2 * membrane[seq2 * 3 + 2] + 
			pixel.v3 * membrane[seq3 * 3 + 2];

		resultColor.setRgb(sourceImage.pixel(x, y));

		resultColor.setRedF(MAX(MIN((resultColor.redF() + r), 1), 0));
		resultColor.setGreenF(MAX(MIN((resultColor.greenF() + g), 1), 0));
		resultColor.setBlueF(MAX(MIN((resultColor.blueF() + b), 1), 0));

		float fr = resultColor.redF();
		float fg = resultColor.greenF();
		float fb = resultColor.blueF();
		blendingResult.setPixel(x, y, resultColor.rgb());
		blendingResultTgt.setPixel(pastePos + QPoint(x, y), resultColor.rgb());


	}

}

void ImageBlending::CalBoundaryDiff()
{
	boundaryDiff.clear();

	for (int i = 0; i < MVCBoundary.size(); ++i)
	{
		QPoint pt_s = MVCBoundary.at(i);
		QPoint pt_t = pt_s + pastePos;

		if (!targetImage.rect().contains(pt_t))
		{
			for (int j = 0; j < 3; ++j)
				boundaryDiff.append(0.0);

			continue;
		}

		QColor color_s = QColor::fromRgb(sourceImage.pixel(pt_s));
		QColor color_t = QColor::fromRgb(targetImage.pixel(pt_t));

		boundaryDiff.append(color_t.redF() - color_s.redF());
		boundaryDiff.append(color_t.greenF() - color_s.greenF());
		boundaryDiff.append(color_t.blueF() - color_s.blueF());
	}

}

//////////////////////////////////////////////////////////////////////////
// Poisson blending
bool ImageBlending::GenBlendMap()
{
	bandMap.clear();
	indexN = 0;

	for (int h = 0; h < blendTrimap.height(); ++h)
	{
		for (int w = 0; w < blendTrimap.width(); ++w)
		{
			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
				continue;

			int id = h * blendTrimap.width() + w;
			if (qGray(blendTrimap.pixel(w, h)) == 128)
			{
				bandMap.insert(id, indexN);
				++indexN;
			}

		}

	}

	if (0 == indexN)
	{
		qDebug() << "there is no blending region";
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	blendMap.clear();
	blendN = 0;

	for (int h = 0; h < blendTrimap.height(); ++h)
	{
		for (int w = 0; w < blendTrimap.width(); ++w)
		{
			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
				continue;

			int id = h * blendTrimap.width() + w;

			if (qGray(blendTrimap.pixel(w, h)) > 100)
			{
				blendMap.insert(id, blendN);
				++blendN;
			}

		}

	}

	if (0 == blendN)
	{
		qDebug() << "there is no blending region";
		return false;
	}

	return true;
	
}

//void ImageBlending::GenHybridGrad()
//{
//	CvSparseMat *L = NULL;
//	int Lsize[2] = {indexN, indexN};
//
//	L = cvCreateSparseMat(2, Lsize, CV_64FC1);
//
//	CvMat *matB = cvCreateMat(indexN, 1, CV_64FC1);	
//	matX = cvCreateMat(indexN, 1, CV_64FC1);
//
//	ConstructMatrixLB(L, matB);
//
//	alphamatting::zhySolver(L, matB, matX);
//
//#if 0
//
//	QImage TestImage(blendTrimap);
//
// 	for (int h = 0; h < sourceImage.height(); ++h)
// 	{
// 		for (int w = 0; w < sourceImage.width(); ++w)
// 		{
// 			int id = h * sourceImage.width() + w;
// 			if (qGray(blendTrimap.pixel(w, h)) == 128)
// 			{
// 				int index = bandMap[id];
//
//				double xx = cvmGet(matX, index, 0);
// 				int iXX;
// 				if (xx >= 1.0) 
//					iXX = 255;
// 				else if (xx <= 0.0) 
//					iXX = 0;
// 				else 
//					iXX = xx * 255.0;
// 
// 				TestImage.setPixel(QPoint(w, h), qRgb(iXX, iXX, iXX));
// 
// 			}
// 		}
// 
// 	}
//
//	TestImage.save("xxHybridGrad.bmp");
//
//#endif
//
//	if (L) cvReleaseSparseMat(&L);
//	if (matB) cvReleaseMat(&matB);
//
//}

void ImageBlending::ConstructMatrixLB(CvSparseMat *L, CvMat *B)
{
	int indexElement = 0;

	for (int h = 0; h < blendTrimap.height(); ++h)
	{
		QRgb* lineScane = (QRgb*)blendTrimap.scanLine(h);
		for (int w = 0; w < blendTrimap.width(); ++w)
		{
			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
				continue;

			double bb = 0.0;	
			int coef = 0;

			int id = 0;
			int idxN;			

			if (128 == qGray(lineScane[w]))
			{
				//////////////////////////////////////////////////////////////////////////				
				// top
				if (targetImage.rect().contains(QPoint(w, h - 1) + pastePos) && h - 1 >= 0)
				{
					--coef;
					int topVal = qGray(blendTrimap.pixel(w, h - 1));
					id = (h - 1) * sourceImage.width() + w;
					idxN = bandMap[id];

					if (128 == topVal)
						cvSetReal2D(L, indexElement, idxN, 1.0f);
					else if (255 == topVal)
						bb -= 1.0;
				}
				
				//////////////////////////////////////////////////////////////////////////
				// left
				
				if (targetImage.rect().contains(QPoint(w - 1, h) + pastePos) && w - 1 >= 0)
				{
					--coef;
					int leftVal = qGray(lineScane[w - 1]);
					id = h * sourceImage.width() + w - 1;
					idxN = bandMap[id];

					if (128 == leftVal)
					{
						cvSetReal2D(L, indexElement, idxN, 1.0f);
					}				
					else if (255 == leftVal)
					{
						bb -= 1.0;
					}
				}			

				//////////////////////////////////////////////////////////////////////////
				// right

				if (targetImage.rect().contains(QPoint(w + 1, h) + pastePos) && w + 1 < blendTrimap.width())
				{
					--coef;
					int rightVal = qGray(lineScane[w + 1]);
					id = h * sourceImage.width() + w + 1;
					idxN = bandMap[id];

					if (128 == rightVal)
					{
						cvSetReal2D(L, indexElement, idxN, 1.0f);
					}				
					else if (255 == rightVal)
					{
						bb -= 1.0;
					}	
				}

				//////////////////////////////////////////////////////////////////////////
				// bottom

				if (targetImage.rect().contains(QPoint(w, h + 1) + pastePos) && h + 1 < blendTrimap.height())
				{
					--coef;
					int bottomVal = qGray(blendTrimap.pixel(w, h + 1));
					id = (h + 1) * sourceImage.width() + w;
					idxN = bandMap[id];

					if (128 == bottomVal)
					{
						cvSetReal2D(L, indexElement, idxN, 1.0f);
					}				
					else if (255 == bottomVal)
					{
						bb -= 1.0;
					}
				}											

				// current pixel
				id = h * sourceImage.width() + w;
				idxN = bandMap[id];

				cvmSet(B, indexElement, 0, bb);
				cvSetReal2D(L, indexElement, idxN, (double)coef);
				++indexElement;

			} // if			

		} // for w

	}// for h

}

void ImageBlending::ConstructMatrixBlending(CvSparseMat *L, CvMat* matB, int _chnl)
{
	int _counts = 0;
	
	for (int h = 0; h < blendTrimap.height(); ++h)
	{
		QRgb* lineScane = (QRgb*)blendTrimap.scanLine(h);

		for (int w = 0; w < blendTrimap.width(); ++w)
		{
			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
				continue;

			double bb = 0.0;
			int id, idxN;
			int coef = 0;

			int grayValue = qGray(lineScane[w]);
			if (grayValue > 100)
			{
				int idxS = sourceImage.width() * h + w;				
				bb = divHybrid[idxS][_chnl];
				
				//////////////////////////////////////////////////////////////////////////
				// top				
				if (targetImage.rect().contains(QPoint(w, h - 1) + pastePos) && h - 1 >= 0)
				{
					--coef;
					int topV = qGray(blendTrimap.pixel(w, h - 1));
					id = (h - 1) * sourceImage.width() + w;
					idxN = blendMap[id];

					if (topV > 100)
						cvSetReal2D(L, _counts, idxN, 1.0f);					
					else
					{
						QRgb tgt = targetImage.pixel(QPoint(w, h - 1) + pastePos);

						if		(0 == _chnl) bb -= qRed(tgt);
						else if (1 == _chnl) bb -= qGreen(tgt);
						else if (2 == _chnl) bb -= qBlue(tgt);

					}

				}									

				//////////////////////////////////////////////////////////////////////////
				// left
				if (targetImage.rect().contains(QPoint(w - 1, h) + pastePos) && w - 1 >= 0)
				{
					--coef;

					int leftV = qGray(blendTrimap.pixel(w - 1, h));
					id = h * sourceImage.width() + w - 1;
					idxN = blendMap[id];

					if (leftV > 100)
						cvSetReal2D(L, _counts, idxN, 1.0f);
					else
					{
						QRgb tgt = targetImage.pixel(QPoint(w - 1, h) + pastePos);

						if		(0 == _chnl) bb -= qRed(tgt);
						else if (1 == _chnl) bb -= qGreen(tgt);
						else if (2 == _chnl) bb -= qBlue(tgt);
					}

				}
				
				//////////////////////////////////////////////////////////////////////////
				// right
				if (targetImage.rect().contains(QPoint(w + 1, h) + pastePos) && w + 1 < blendTrimap.width())
				{
					--coef;

					int rightV = qGray(blendTrimap.pixel(w + 1, h));
					id = h * sourceImage.width() + w + 1;
					idxN = blendMap[id];

					if (rightV > 100)
						cvSetReal2D(L, _counts, idxN, 1.0f);
					else
					{
						QRgb tgt = targetImage.pixel(QPoint(w + 1, h) + pastePos);

						if		(0 == _chnl) bb -= qRed(tgt);
						else if (1 == _chnl) bb -= qGreen(tgt);
						else if (2 == _chnl) bb -= qBlue(tgt);
					}
					
				}			
				
				//////////////////////////////////////////////////////////////////////////
				// bottom
				if (targetImage.rect().contains(QPoint(w, h + 1) + pastePos) && h  + 1 < blendTrimap.height())
				{
					--coef;
					int bottomV = qGray(blendTrimap.pixel(w, h + 1));
					id = (h + 1) * sourceImage.width() + w;
					idxN = blendMap[id];

					if (bottomV > 100)
						cvSetReal2D(L, _counts, idxN, 1.0f);
					else
					{
						QRgb tgt = targetImage.pixel(QPoint(w, h + 1) + pastePos);

						if		(0 == _chnl) bb -= qRed(tgt);
						else if (1 == _chnl) bb -= qGreen(tgt);
						else if (2 == _chnl) bb -= qBlue(tgt);
					}
					
				}								

				int cutId = h * sourceImage.width() + w;
				cvSetReal2D(L, _counts, blendMap[cutId], (double)coef);
				cvmSet(matB, _counts, 0, bb);

				++_counts;

			}// if

		} // for w

	} // for h

}

//void ImageBlending::CalculateHybridGrad()
//{
//	// source image
//	int _width = blendTrimap.width();
//	int _height = blendTrimap.height();
//
//	for (int h = 0; h < _height; ++h)
//	{
//		for (int w = 0; w < _width; ++w)
//		{
//			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
//				continue;
//
//			// for trimap foreground
//			int iTrimap = qGray(blendTrimap.pixel(w, h));
//
//			if (iTrimap > 100)
//			{
//				QRgb xy_pixS = sourceImage.pixel(QPoint(w, h));
//				QRgb xy_pixT = targetImage.pixel(QPoint(w, h) + pastePos);
//
//				QRgb x_pix, y_pix;
//
//				double X_gradS_r = .0, X_gradS_g = .0, X_gradS_b = .0;
//				double Y_gradS_r = .0, Y_gradS_g = .0, Y_gradS_b = .0;
//
//				double X_gradT_r = .0, X_gradT_g = .0, X_gradT_b = .0;
//				double Y_gradT_r = .0, Y_gradT_g = .0, Y_gradT_b = .0;
//
//				if (targetImage.rect().contains(QPoint(w + 1, h) + pastePos) && (w + 1) < _width)
//				{
//					// source					
//					x_pix = sourceImage.pixel(QPoint(w + 1, h));
//					X_gradS_r = qRed(x_pix) - qRed(xy_pixS);
//					X_gradS_g = qGreen(x_pix) - qGreen(xy_pixS);
//					X_gradS_b = qBlue(x_pix) - qBlue(xy_pixS);
//
//					// target
//					x_pix = targetImage.pixel(QPoint(w + 1, h) + pastePos);
//					X_gradT_r = qRed(x_pix) - qRed(xy_pixT);
//					X_gradT_g = qGreen(x_pix) - qGreen(xy_pixT);
//					X_gradT_b = qBlue(x_pix) - qBlue(xy_pixT);
//
//				}
//
//				if (targetImage.rect().contains(QPoint(w, h + 1) + pastePos) && (h + 1) < _height)
//				{
//					// source
//					y_pix = sourceImage.pixel(QPoint(w, h + 1));
//					Y_gradS_r = qRed(y_pix) - qRed(xy_pixS);
//					Y_gradS_g = qGreen(y_pix) - qGreen(xy_pixS);
//					Y_gradS_b = qBlue(y_pix) - qBlue(xy_pixS);
//
//					// target
//					y_pix = targetImage.pixel(QPoint(w, h + 1) + pastePos);
//					Y_gradT_r = qRed(y_pix) - qRed(xy_pixT);
//					Y_gradT_g = qGreen(y_pix) - qGreen(xy_pixT);
//					Y_gradT_b = qBlue(y_pix) - qBlue(xy_pixT);
//				}
//
//				int index = h * _width + w;
//				double _alpha = cvmGet(matX, bandMap[index], 0);				
//				
//				QVector<QPointF> _hybridGrad;
//				QPointF pt = QPoint(w, h) + pastePos;
//				if (pt.x() == 0 || pt.x() == targetImage.width() - 1 ||
//					pt.y() == 0 || pt.y() == targetImage.height() - 1)
//				{
//					// for points on the boundary, their gradient = {0, 0}
// 					_hybridGrad.append(QPointF(0.0, 0.0));
// 					_hybridGrad.append(QPointF(0.0, 0.0));
// 					_hybridGrad.append(QPointF(0.0, 0.0));
//				}
//				else
//				{					
//					if (iTrimap == 128)
//					{
// 						if (qGray(salientTrimap.pixel(w, h)) > 200)
// 						{
//  							double valGradS = sqrDist(X_gradS_r, Y_gradS_r) + 
//  											  sqrDist(X_gradS_g, Y_gradS_g) + 
//  											  sqrDist(X_gradS_b, Y_gradS_b);
//  
//  							double valGradT = sqrDist(X_gradT_r, Y_gradT_r) + 
//  											  sqrDist(X_gradT_g, Y_gradT_g) + 
//  											  sqrDist(X_gradT_b, Y_gradT_b);
//  
//  							if (valGradS > valGradT)
//  							{
//  								_hybridGrad.append(QPointF(X_gradS_r, Y_gradS_r));
//  								_hybridGrad.append(QPointF(X_gradS_g, Y_gradS_g));
//  								_hybridGrad.append(QPointF(X_gradS_b, Y_gradS_b));
//  							}
//  							else
//  							{
//  								_hybridGrad.append(QPointF(X_gradT_r, Y_gradT_r));
//  								_hybridGrad.append(QPointF(X_gradT_g, Y_gradT_g));
//  								_hybridGrad.append(QPointF(X_gradT_b, Y_gradT_b));
//  							}
// 							
// 						}
// 						else
//						{
//							_hybridGrad.append(QPointF(X_gradT_r * (1 - _alpha) + X_gradS_r * _alpha, Y_gradT_r * (1 - _alpha) + Y_gradS_r * _alpha));
//							_hybridGrad.append(QPointF(X_gradT_g * (1 - _alpha) + X_gradS_g * _alpha, Y_gradT_g * (1 - _alpha) + Y_gradS_g * _alpha));
//							_hybridGrad.append(QPointF(X_gradT_b * (1 - _alpha) + X_gradS_b * _alpha, Y_gradT_b * (1 - _alpha) + Y_gradS_b * _alpha));
//						}
//
//					}
//					else if (iTrimap == 255)
//					{
//						if (qGray(salientTrimap.pixel(w, h)) > 200)
//						{
//							double valGradS = sqrDist(X_gradS_r, Y_gradS_r) + 
//								sqrDist(X_gradS_g, Y_gradS_g) + 
//								sqrDist(X_gradS_b, Y_gradS_b);
//
//							double valGradT = sqrDist(X_gradT_r, Y_gradT_r) + 
//								sqrDist(X_gradT_g, Y_gradT_g) + 
//								sqrDist(X_gradT_b, Y_gradT_b);
//
//							if (valGradS > valGradT)
//							{
//								_hybridGrad.append(QPointF(X_gradS_r, Y_gradS_r));
//								_hybridGrad.append(QPointF(X_gradS_g, Y_gradS_g));
//								_hybridGrad.append(QPointF(X_gradS_b, Y_gradS_b));
//							}
//							else
//							{
//								_hybridGrad.append(QPointF(X_gradT_r, Y_gradT_r));
//								_hybridGrad.append(QPointF(X_gradT_g, Y_gradT_g));
//								_hybridGrad.append(QPointF(X_gradT_b, Y_gradT_b));
//							}
//
//						}
//						else
//						{
//							_hybridGrad.append(QPointF(X_gradS_r, Y_gradS_r));
//							_hybridGrad.append(QPointF(X_gradS_g, Y_gradS_g));
//							_hybridGrad.append(QPointF(X_gradS_b, Y_gradS_b));
//						}
//
//					}
//
//				} // if not on the boundary
//
//				gradHybrid.insert(index, _hybridGrad);
//
//			} // if iTrimap > 100
//
//			//////////////////////////////////////////////////////////////////////////
//#if 0
//
//			int leftTri = qGray(blendTrimap.pixel(w - 1, h));
//			int rightTri = qGray(blendTrimap.pixel(w + 1, h));
//			int topTri = qGray(blendTrimap.pixel(w, h - 1));
//			int btmTri = qGray(blendTrimap.pixel(w, h + 1));
//
//			// if the point is on the boundary
//			if (iTrimap > 100 || (iTrimap > 100 && leftTri == 0) || (iTrimap > 100 && rightTri == 0) ||
//				(iTrimap > 100 && topTri == 0) || (iTrimap > 100 && btmTri == 0))
//			{
//				// source
//				QRgb _x = sourceImage.pixel(QPoint(w + 1, h));
//				QRgb _y = sourceImage.pixel(QPoint(w, h + 1));
//				QRgb xy = sourceImage.pixel(QPoint(w, h));
//
//				int s_gradxR = qRed(_x) - qRed(xy);
//				int s_gradxG = qGreen(_x) - qGreen(xy);
//				int s_gradxB = qBlue(_x) - qBlue(xy);
//
//				int s_gradyR = qRed(_y) - qRed(xy);
//				int s_gradyG = qGreen(_y) - qGreen(xy);
//				int s_gradyB = qBlue(_y) - qBlue(xy);
//
//				// target
//				_x = targetImage.pixel(QPoint(w + 1, h) + pastePos);
//				_y = targetImage.pixel(QPoint(w, h + 1) + pastePos);
//				xy = targetImage.pixel(QPoint(w, h) + pastePos);
//
//				double t_gradxR = qRed(_x) - qRed(xy);
//				double t_gradxG = qGreen(_x) - qGreen(xy);
//				double t_gradxB = qBlue(_x) - qBlue(xy);
//
//				double t_gradyR = qRed(_y) - qRed(xy);
//				double t_gradyG = qGreen(_y) - qGreen(xy);
//				double t_gradyB = qBlue(_y) - qBlue(xy);
//
//				//////////////////////////////////////////////////////////////////////////
//				// hybrid
//				int index = h * _width + w;
//				double _alpha = cvmGet(matX, bandMap[index], 0);
//
//				QVector<QPointF> _hybridGrad;
//				if (leftTri == 0 || rightTri == 0 || topTri == 0 || btmTri == 0)
//				{
//					_hybridGrad.append(QPointF(t_gradxR, t_gradyR));
//					_hybridGrad.append(QPointF(t_gradxG, t_gradyG));
//					_hybridGrad.append(QPointF(t_gradxB, t_gradyB));
//
//				}
//				else if (iTrimap == 128)
//				{
// 					_hybridGrad.append(QPointF(t_gradxR * (1 - _alpha) + s_gradxR * _alpha, t_gradyR * (1 - _alpha) + s_gradyR * _alpha));
// 					_hybridGrad.append(QPointF(t_gradxG * (1 - _alpha) + s_gradxG * _alpha, t_gradyG * (1 - _alpha) + s_gradyG * _alpha));
// 					_hybridGrad.append(QPointF(t_gradxB * (1 - _alpha) + s_gradxB * _alpha, t_gradyB * (1 - _alpha) + s_gradyB * _alpha));
//
//// 					if (_alpha <= 0.4)
//// 					{
//// 						_hybridGrad.append(QPointF(t_gradxR, t_gradyR));
//// 						_hybridGrad.append(QPointF(t_gradxG, t_gradyB));
//// 						_hybridGrad.append(QPointF(t_gradxG, t_gradyB));
//// 					}
//// 					else
//// 					{	
//// 						_hybridGrad.append(QPointF(s_gradxR, s_gradyR));
//// 						_hybridGrad.append(QPointF(s_gradxG, s_gradyG));
//// 						_hybridGrad.append(QPointF(s_gradxB, s_gradyB));
//// 					}
//
//				}
//				else if (iTrimap == 255)
//				{
//					_hybridGrad.append(QPointF(s_gradxR, s_gradyR));
//					_hybridGrad.append(QPointF(s_gradxG, s_gradyG));
//					_hybridGrad.append(QPointF(s_gradxB, s_gradyB));
//
//				}
//
//				gradHybrid.insert(index, _hybridGrad);
//			}
//#endif
//
//		}
//
//	}
//
//}

//void ImageBlending::CalculateHybridDiv()
//{
//	gradHybrid.clear();
//	divHybrid.clear();
//
//	CalculateHybridGrad();
//
//	int _width = sourceImage.width();
//	int _height = sourceImage.height();
//
//	for (int h = 0; h < _height; ++h)
//	{
//		QRgb* lineScane = (QRgb*)blendTrimap.scanLine(h);
//
//		for (int w = 0; w < _width; ++w)
//		{
//			if (!targetImage.rect().contains(QPoint(w, h) + pastePos))
//				continue;
//
//			// foreground and the trimap band
//			if (qGray(lineScane[w]) > 100)
//			{
//				int _index = h * _width + w;
//				double gradXX = .0;
//				double gradYY = .0;
//
//				double div_x[3] = {.0};
//				if (targetImage.rect().contains(QPoint(w - 1, h) + pastePos) && (w - 1) >= 0)
//				{
//					int _xx = _index - 1;
//
//					for (int i = 0; i < 3; ++i)
//						div_x[i] = gradHybrid[_index][i].x() - gradHybrid[_xx][i].x();										
//				}
//
//				double div_y[3] = {.0};
//				if (targetImage.rect().contains(QPoint(w, h - 1) + pastePos) && (h - 1) >= 0)
//				{
//					int _yy = (h - 1) * _width + w;
//
//					for (int i = 0; i < 3; ++i)
//						div_y[i] = gradHybrid[_index][i].y() - gradHybrid[_yy][i].y();
//				}
//
//				QVector<double> _div;
//
//				for (int i = 0; i < 3; ++i)
//					_div.append(div_x[i] + div_y[i]);
//
//				divHybrid.insert(_index, _div);	
//			
//			}
//
//		}
//
//	}
//
//}

void ImageBlending::RegionTriangulate()
{
	TriangleList.clear();
	
	Triangulate_2d(MVCVertex, TriangleList);

	MVCoord.PointSize = MVCVertex.size();
	MVCoord.BoundarySize = MVCBoundary.size();

	if (MVCoord.mvcArray) delete MVCoord.mvcArray;	
	MVCoord.mvcArray = new double[MVCoord.PointSize * MVCoord.BoundarySize];

	QVector<int> boundaryIndex;
	
	for (int i = 0; i < MVCBoundary.size(); ++i)
	{
		QPoint pt = MVCBoundary[i];

		if (targetImage.rect().contains(pt + pastePos))
		{
			boundaryIndex.append(i);
		}
	}

	CalculateMVCoord(&MVCBoundary, &MVCoord, &MVCVertex, boundaryIndex);
	TriRegionPoint.clear();
	findTriangle(TriRegionPoint, &MVCVertex, &TriangleList, sourceImage.width());
}

//double ImageBlending::SumOFDiff()
//{
//	double sum = 0;
//	
//	for (int i = 0; i < MVCBoundary.size(); ++i)
//	{		
//		QPoint pt_t = MVCBoundary.at(i) + pastePos;
//
//		if (!targetImage.rect().contains(pt_t))
//		{
//			for (int j = 0; j < 3; ++j)
//				boundaryDiff.append(0.0);
//
//			continue;
//		}
//
//		QColor color_r = QColor::fromRgb(blendingHybridRst.pixel(pt_t));
//		QColor color_t = QColor::fromRgb(targetImage.pixel(pt_t));
//
//		//boundaryDiff.append(color_t.redF() - color_s.redF());
//		//boundaryDiff.append(color_t.greenF() - color_s.greenF());
//		//boundaryDiff.append(color_t.blueF() - color_s.blueF());
//
//		sum += fabs(color_t.redF() - color_r.redF());
//		sum += fabs(color_t.greenF() - color_r.greenF());
//		sum += fabs(color_t.blueF() - color_r.blueF());
//
//	}
//
//	return sum / MVCBoundary.size();
//
//}

//double ImageBlending::HybridGradBlending(const Mat& _alpha, double _blendRate, const QPoint& _pastePt)
//{
//	// generate the hybrid gradient map
//
//	pastePos = _pastePt;
//	
//	GenBlendMap();
//	GenHybridGrad();
//	CalculateHybridDiv();
//
//	CvMat* matImgR = cvCreateMat(blendN, 1, CV_64FC1);
//	CvMat* matImgG = cvCreateMat(blendN, 1, CV_64FC1);
//	CvMat* matImgB = cvCreateMat(blendN, 1, CV_64FC1);
//	CvMat *matB = cvCreateMat(blendN, 1, CV_64FC1);	
//
//	int Lsize[2] = {blendN, blendN};
//	
//	// red channel
//	CvSparseMat *L = cvCreateSparseMat(2, Lsize, CV_64FC1);
// 	ConstructMatrixBlending(L, matB, 0);
// 	alphamatting::zhySolver(L, matB, matImgR);
// 	if (L) cvReleaseSparseMat(&L);
//
//	// green channel
// 	L = cvCreateSparseMat(2, Lsize, CV_64FC1);	
//	ConstructMatrixBlending(L, matB, 1);
//	alphamatting::zhySolver(L, matB, matImgG);
//	if (L) cvReleaseSparseMat(&L);
//
//	// blue channel
//	L = cvCreateSparseMat(2, Lsize, CV_64FC1);	
//	ConstructMatrixBlending(L, matB, 2);
//	alphamatting::zhySolver(L, matB, matImgB);
//	if (L) cvReleaseSparseMat(&L);	
// 
//	// get the blending result
//	blendingHybridRst = QImage(targetImage);
//
//	for (int h = 0; h < blendTrimap.height(); ++h)
//	{
//		for (int w = 0; w < blendTrimap.width(); ++w)
//		{
//			QPoint tgtPt = QPoint(w, h) + pastePos;
//
//			if (!targetImage.rect().contains(tgtPt))
//				continue;
//
//			int id = h * blendTrimap.width() + w;
//			int grayValue = qGray(blendTrimap.pixel(w, h));
//
//			if (grayValue > 100)
//			{		
//				int r = cvmGet(matImgR, blendMap[id], 0);
//				int g = cvmGet(matImgG, blendMap[id], 0);
//				int b = cvmGet(matImgB, blendMap[id], 0);
//
//// 				double offset_r = r - qRed(sourceImage.pixel(QPoint(w, h)));
//// 				double offset_g = g - qGreen(sourceImage.pixel(QPoint(w, h)));
//// 				double offset_b = b - qBlue(sourceImage.pixel(QPoint(w, h)));
//
//// 				if (_alpha.at<uchar>(h, w) > 200)
//// 				{
//// 					double dlAlpha = (double)(_alpha.at<uchar>(h, w)) / 255.0;
//// 
//// 					r = offset_r * _blendRate * dlAlpha + qRed(sourceImage.pixel(QPoint(w, h)));
//// 					g = offset_g * _blendRate * dlAlpha + qGreen(sourceImage.pixel(QPoint(w, h)));
//// 					b = offset_b * _blendRate * dlAlpha + qBlue(sourceImage.pixel(QPoint(w, h)));
//// 				}
//
//				if (r > 255) r = 255;
//				if (g > 255) g = 255;
//				if (b > 255) b = 255;
//
//				if (r < 0) r = 0;
//				if (g < 0) g = 0;
//				if (b < 0) b = 0;
//
//				blendingHybridRst.setPixel(tgtPt, qRgb(r, g, b));
//			
//			}
//
//		}
//
//	}
//
//	
//
//	cvReleaseSparseMat(&L);
//	cvReleaseMat(&matImgR);
//	cvReleaseMat(&matImgG);
//	cvReleaseMat(&matImgB);
//	cvReleaseMat(&matB);
//
//	return SumOFDiff();
//}


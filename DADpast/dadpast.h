#ifndef DADPAST_H
#define DADPAST_H
   
#include <QMainWindow>
#include "ui_dadpast.h"
#include <opencv2/opencv.hpp>
#include "qimage.h"
#include "qmessagebox.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include "Screen.h"
#include "IplImageOperation.h"
#include "dadGraph.h"
#include "imageblending.h"

using namespace std;
using namespace cv;


class DADpast: public QMainWindow
{
	Q_OBJECT

public:
	Screen* screen;
	Screen* screen2;
	DADGraph* dadgraph;

	QString srcFile;
	QString tarFile;

	QImage src;
	QImage target;
	QImage result;

	IplImage* isrc;
	IplImage* itarget;
	IplImage* isrcOn;
	IplImage* itargetOn;
	IplImage* tmp;

    IplImage* itrimap;

	CvPoint maskup, maskdown;
	CvPoint cutLineP1, cutLineP2;
	CvPoint startP1, startP2;
	CvPoint endP1, endP2;

	QVector<QPoint> contour;
	float radius;  // painter size
    int x0, y0;    // location of source image
	int mouse_x, mouse_y; // location where mouse clicked at first
	int WIDTH, HEIGHT;

	DADpast(QWidget *parent = 0);
	~DADpast();

    void mouseMoveEvent(QMouseEvent* ev);
	void Repaint();
    void getMask();
private:
	Ui::DADpastClass ui;

public slots:
	
	void readSrc();
	void readTarget();
	QString getFile(QString info);
	QImage* IplImagetoQImage(IplImage *img);
	IplImage* QImagetoIplImage(QImage *qImage);
	void ImageBlend();
	void initDraw();
	void DragDrop();

	void showImg();
	void mouseMove(QPoint& p);
	void mousePress(QPoint& p){
		mouse_x = p.x();
		mouse_y = p.y();
	}
	void mouseRelease(QPoint& p){
		x0 += p.x() - mouse_x;
		y0 += p.y() - mouse_y;
	}
};

#endif // DADPAST_H

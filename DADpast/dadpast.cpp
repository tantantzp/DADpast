#include "dadpast.h"
#include <opencv2/opencv.hpp>
#include <qfiledialog.h>
#include <qdebug.h>


using namespace std;
using namespace cv;

   
DADpast::DADpast(QWidget *parent)
	: QMainWindow(parent)
{
	
	ui.setupUi(this);
	QGridLayout *Mainlayout = new QGridLayout;
    Mainlayout->setContentsMargins(0, 70, 0, 0);
	screen = new Screen();
	screen2 = new Screen();

    QScrollArea *scrollArea = new QScrollArea;
	scrollArea->setWidget(screen);
    Mainlayout->addWidget(scrollArea);
	
	QScrollArea *scrollArea2 = new QScrollArea;
	scrollArea2->setWidget(screen2);
    Mainlayout->addWidget(scrollArea2);
	ui.centralWidget->setLayout(Mainlayout);

	radius = 15.0; 
	isrc = NULL;
	itarget = NULL;
	tmp = NULL;
	itargetOn = NULL;
	isrcOn = NULL;
	WIDTH = 800;
	HEIGHT = 500;

	QObject::connect(ui.actionInput_src, SIGNAL(triggered()), this, SLOT(readSrc()));
	QObject::connect(ui.actionInput_target, SIGNAL(triggered()), this, SLOT(readTarget()));
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(initDraw()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(DragDrop()));
	QObject::connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(ImageBlend()));

	QObject::connect(screen, SIGNAL(mouseMove(QPoint&)), this, SLOT(mouseMove(QPoint&)));
	QObject::connect(screen, SIGNAL(mousePress(QPoint&)), this, SLOT(mousePress(QPoint&)));
	QObject::connect(screen, SIGNAL(mouseRelease(QPoint&)), this, SLOT(mouseRelease(QPoint&)));
	QObject::connect(screen2, SIGNAL(mouseMove(QPoint&)), this, SLOT(mouseMove(QPoint&)));
	QObject::connect(screen2, SIGNAL(mousePress(QPoint&)), this, SLOT(mousePress(QPoint&)));
	QObject::connect(screen2, SIGNAL(mouseRelease(QPoint&)), this, SLOT(mouseRelease(QPoint&)));

}

DADpast::~DADpast()
{

}

void DADpast::mouseMove(QPoint& p){
	if( isrcOn != NULL && itargetOn != NULL) 
	{
		
		for (int i=p.x()-radius; i<p.x()+radius; i++){
			for (int j=p.y()-radius; j<p.y()+radius; j++){
				SetPixel3C(isrcOn, i, j, 150, 150, 150);
				SetPixel3C(itargetOn, i, j, 150, 150, 150);
				SetPixel1C(itrimap, i, j, 100);
			}
		}
		showImg(); 
	}
	

}
void DADpast::mouseMoveEvent(QMouseEvent *ev){
	
}

void DADpast::readSrc(){
	srcFile = getFile("read src");
	if (srcFile == NULL)
		return;
	QImage tmpSrc;
	tmpSrc = QImage(srcFile);
	src = tmpSrc.scaled(WIDTH, HEIGHT);

	//src = QImage(filename);
	isrc = QImagetoIplImage(&src);
	isrcOn = cvCloneImage(isrc);
	itrimap = cvCreateImage(cvSize(isrc->width, isrc->height), IPL_DEPTH_8U, 1);

	showImg();
}

void DADpast::readTarget(){
	tarFile = getFile("read target");
	if (tarFile == NULL)
		return;
	QImage tmpTar;
	tmpTar = QImage(tarFile);
	target = tmpTar.scaled(WIDTH, HEIGHT);
	//target = QImage(filename);
	itarget = QImagetoIplImage(&target);
	itargetOn = cvCloneImage(itarget);
	showImg();
}

void DADpast::initDraw(){
	isrcOn = cvCloneImage(isrc);
    itargetOn = cvCloneImage(itarget);
	itrimap = cvCreateImage(cvSize(isrc->width, isrc->height), IPL_DEPTH_8U, 1);
	contour.clear();
	radius = 15.0;
	showImg();
}

void DADpast::DragDrop(){
	cout << "dragdrop"<< endl;

	dadgraph = new DADGraph(isrc, itarget, itrimap);
	
	DADPath path = dadgraph->getPath();

	isrcOn = cvCloneImage(isrc);
    itargetOn = cvCloneImage(itarget);
	for(int i = 0; i < path.Path.size(); i++)
	{
		//cout << path.Path[i].x << ' '<< path.Path[i].y << ';';
		contour.append(QPoint(path.Path[i].y, path.Path[i].x));
		SetPixel3C(isrcOn,path.Path[i].y, path.Path[i].x, 150, 150, 150);
		SetPixel3C(itargetOn,path.Path[i].y, path.Path[i].x, 150, 150, 150);
	}



	showImg();
}

void DADpast::ImageBlend()
{
	ImageBlending ib(src,target,contour);
	ib.MVCBlending(QPoint(0,0));
	ib.blendingResultTgt.save("r.png");
	result = QImage("r.png");
	screen2->setPixmap(QPixmap::fromImage(result));
}



void DADpast::Repaint(){
	if (isrc == NULL || itarget == NULL)
		return;

	showImg();
}


void DADpast::showImg(){
	if(isrcOn != NULL)
	{
		screen->resize(isrcOn->width, isrcOn->height);
	    screen->setPixmap(QPixmap::fromImage(*IplImagetoQImage(isrcOn)));
		cvCvtColor(isrcOn, isrcOn, CV_RGB2BGR);
	}
	if(itargetOn != NULL)
	{
	    screen2->resize(itargetOn->width, itargetOn->height);
	    screen2->setPixmap(QPixmap::fromImage(*IplImagetoQImage(itargetOn)));
	    cvCvtColor(itargetOn, itargetOn, CV_RGB2BGR);
	}

}


QString DADpast::getFile(QString info){
	QString filename = QFileDialog::getOpenFileName(
		 this,
		info,
		QDir::currentPath(),
		"Image files (*.png *.jpg);;All files(*.*)");
	return filename;
}

QImage* DADpast::IplImagetoQImage(IplImage *tmp){
	QImage *image = NULL;
	//IplImage* tmp = cvCloneImage(img);
	int n = tmp->nChannels;

	if (n == 1){
		cvCvtColor(tmp, tmp, CV_GRAY2RGB);
	}if (n == 3){
		cvCvtColor(tmp, tmp, CV_BGR2RGB);
	}
	uchar *imgdata = (uchar*) tmp->imageData;
	image = new QImage(imgdata, tmp->width, tmp->height, QImage::Format_RGB888);
	return image;
}


IplImage* DADpast::QImagetoIplImage(QImage *qImage){
	int width = qImage->width();   
    int height = qImage->height();   
    CvSize Size;   
    Size.height = height;   
    Size.width = width;   
    IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);   
    for (int y = 0; y < height; ++y)   
    {   
        for (int x = 0; x < width; ++x)   
        {   
            QRgb rgb = qImage->pixel(x, y);   
            cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));   
        }   
    }   
    return IplImageBuffer;   
}

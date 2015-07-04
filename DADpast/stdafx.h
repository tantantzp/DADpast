   
#include <vector>
#include <set>
#include <iostream>
#include <map>
#include <queue>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <sstream>

#include <algorithm>

#include <Windows.h>
#include <ShellAPI.h>
#include <omp.h>


// OpenGL

//#include <gl/GL.h>
// #include <gl/glut.h>
// #include <gl/GLAux.h>
//  
// #pragma comment(lib, "glaux.lib")
// #pragma comment(lib, "glut32.lib")
// // matlab
// #include <engine.h> 
// #pragma comment(lib, "libmx.lib")
// #pragma comment(lib, "libeng.lib")

// QT

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QObject>

// OpenCV 2.4
#include <opencv2/opencv.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_core244d.lib")
#pragma comment(lib, "opencv_highgui244d.lib")
#pragma comment(lib, "opencv_imgproc244d.lib")
#pragma comment(lib, "opencv_video244d.lib")
#pragma comment(lib, "opencv_features2d244d.lib")
#else
#pragma comment(lib, "opencv_core244.lib")
#pragma comment(lib, "opencv_highgui244.lib")
#pragma comment(lib, "opencv_imgproc244.lib")
#pragma comment(lib, "opencv_video244.lib")
#pragma comment(lib, "opencv_features2d244.lib")
#endif // _DEBUG

//using namespace cv;

#define SET_FONT_GREEN SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN)
#define SET_FONT_RED SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED)
#define SET_FONT_BLUE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE)
#define SET_FONT_YELLOW SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY |FOREGROUND_RED | FOREGROUND_GREEN)
#define SET_FONT_MAGENTA SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE)
#define SET_FONT_CYAN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define SET_FONT_WHITE SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)

#define FOR_INTLOOP_ADD1(x, left, right) for (int x = left; x < right; ++x)
#define FOR_INTLOOP_SUB1(x, left, right) for (int x = left; x > right; --x)

#define CONSOLE_WARNING_QT(x) SET_FONT_YELLOW;\
	qDebug() << x;\
	SET_FONT_WHITE

#define CONSOLE_TESTING_QT(x) SET_FONT_GREEN;\
	qDebug() << x;\
	SET_FONT_WHITE


#define IN
#define OUT

// head files for current project

//#include "segmentation/segment-image.h"
//#include "graphCut/graph.h"
//#include "graphCut/adjacency_list/graph.h"

//#include "graphCut/energy.h"
// typedef Graph<double, double, double> GraphType;
// const double DBL_BIG = 10.0;
// const double e = 2.71828183;

// GMM
//#include "GMM/CmDefinition.h"
//#include "GMM/CmLog.h"
//#include "GMM/CmSetting.h"
//#include "GMM/CmUFSet.h"
//#include "GMM/CmCv.h"
//#include "GMM/CmFile.h"
//#include "GMM/CmComand.h"
//
//#include "GMM/CmIllustr.h"
//#include "GMM/CmGMM_.h"
//
//
//#include "matting/global.h"

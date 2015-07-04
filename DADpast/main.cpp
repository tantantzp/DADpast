#include "dadpast.h"
#include <QApplication>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
  
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DADpast w;
	w.show();
	return a.exec();
}

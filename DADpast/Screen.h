#ifndef SCREEN_H
#define SCREEN_H
  
#include <QMainWindow>
#include <qlabel.h>
#include <QMouseEvent>
#include <qdebug.h>



class Screen:public QLabel{
    Q_OBJECT
public:
	Screen(QWidget *parent = 0, Qt::WindowFlags f=0){
		QLabel(parent, f);
	}
	~Screen(){}

	void mouseMoveEvent(QMouseEvent* ev){
		QPoint p = ev->pos();
		emit mouseMove(p);
	}
	
	void mousePressEvent(QMouseEvent* ev){
		QPoint p = ev->pos();
		emit mousePress(p);
	}
	void mouseReleaseEvent(QMouseEvent* ev){
		QPoint p = ev->pos();
		emit mouseRelease(p);
	}
signals:
	void mouseMove(QPoint &p);
	void mousePress(QPoint &p);
	void mouseRelease(QPoint &p);
};

#endif
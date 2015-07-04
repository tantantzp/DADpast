/********************************************************************************
** Form generated from reading UI file 'dadpast.ui'
**
** Created: Wed May 14 21:49:25 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DADPAST_H
#define UI_DADPAST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DADpastClass
{
public:
    QAction *actionInput_src;
    QAction *actionInput_target;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DADpastClass)
    {
        if (DADpastClass->objectName().isEmpty())
            DADpastClass->setObjectName(QString::fromUtf8("DADpastClass"));
        DADpastClass->resize(584, 400);
        actionInput_src = new QAction(DADpastClass);
        actionInput_src->setObjectName(QString::fromUtf8("actionInput_src"));
        actionInput_target = new QAction(DADpastClass);
        actionInput_target->setObjectName(QString::fromUtf8("actionInput_target"));
        centralWidget = new QWidget(DADpastClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(280, 30, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(370, 30, 75, 23));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(460, 30, 75, 23));
        DADpastClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DADpastClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 584, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        DADpastClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DADpastClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        DADpastClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DADpastClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DADpastClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionInput_src);
        menuFile->addAction(actionInput_target);

        retranslateUi(DADpastClass);
        QObject::connect(pushButton, SIGNAL(clicked()), DADpastClass, SLOT(show()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), DADpastClass, SLOT(show()));

        QMetaObject::connectSlotsByName(DADpastClass);
    } // setupUi

    void retranslateUi(QMainWindow *DADpastClass)
    {
        DADpastClass->setWindowTitle(QApplication::translate("DADpastClass", "DADpast", 0, QApplication::UnicodeUTF8));
        actionInput_src->setText(QApplication::translate("DADpastClass", "input src", 0, QApplication::UnicodeUTF8));
        actionInput_target->setText(QApplication::translate("DADpastClass", "input target", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DADpastClass", "draw", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DADpastClass", "ok", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("DADpastClass", "blend", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("DADpastClass", "file", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DADpastClass: public Ui_DADpastClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DADPAST_H

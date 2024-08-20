/********************************************************************************
** Form generated from reading UI file 'emgwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMGWIDGET_H
#define UI_EMGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EMGWidget
{
public:
    QAction *actionSave;
    QAction *actionOpen;
    QAction *actionClear_plot;
    QAction *actionPlot_color;
    QAction *actionDevice_info;
    QAction *sensorNumber;
    QAction *actionClear_log;
    QAction *actionClear_all;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_ConnectDisconnect;
    QComboBox *cb_COMP;
    QVBoxLayout *verticalLayout;
    QCustomPlot *customPlot;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuAbout;

    void setupUi(QMainWindow *EMGWidget)
    {
        if (EMGWidget->objectName().isEmpty())
            EMGWidget->setObjectName("EMGWidget");
        EMGWidget->resize(807, 639);
        actionSave = new QAction(EMGWidget);
        actionSave->setObjectName("actionSave");
        actionOpen = new QAction(EMGWidget);
        actionOpen->setObjectName("actionOpen");
        actionClear_plot = new QAction(EMGWidget);
        actionClear_plot->setObjectName("actionClear_plot");
        actionPlot_color = new QAction(EMGWidget);
        actionPlot_color->setObjectName("actionPlot_color");
        actionDevice_info = new QAction(EMGWidget);
        actionDevice_info->setObjectName("actionDevice_info");
        sensorNumber = new QAction(EMGWidget);
        sensorNumber->setObjectName("sensorNumber");
        actionClear_log = new QAction(EMGWidget);
        actionClear_log->setObjectName("actionClear_log");
        actionClear_all = new QAction(EMGWidget);
        actionClear_all->setObjectName("actionClear_all");
        centralwidget = new QWidget(EMGWidget);
        centralwidget->setObjectName("centralwidget");
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btn_ConnectDisconnect = new QPushButton(centralwidget);
        btn_ConnectDisconnect->setObjectName("btn_ConnectDisconnect");

        horizontalLayout->addWidget(btn_ConnectDisconnect);

        cb_COMP = new QComboBox(centralwidget);
        cb_COMP->setObjectName("cb_COMP");
        cb_COMP->setEnabled(true);
        cb_COMP->setMinimumSize(QSize(0, 0));
        cb_COMP->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(cb_COMP);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName("customPlot");
        customPlot->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);
        customPlot->setMinimumSize(QSize(0, 400));
        customPlot->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout->addWidget(customPlot);

        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName("textBrowser");
        textBrowser->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);
        textBrowser->setMaximumSize(QSize(16777215, 100));

        verticalLayout->addWidget(textBrowser);


        gridLayout_2->addLayout(verticalLayout, 1, 0, 1, 1);

        EMGWidget->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(EMGWidget);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 807, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName("menuFile");
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName("menuEdit");
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName("menuAbout");
        EMGWidget->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionOpen);
        menuEdit->addAction(actionClear_plot);
        menuEdit->addAction(actionClear_log);
        menuEdit->addAction(actionClear_all);
        menuEdit->addAction(actionPlot_color);
        menuEdit->addAction(sensorNumber);
        menuAbout->addAction(actionDevice_info);

        retranslateUi(EMGWidget);

        QMetaObject::connectSlotsByName(EMGWidget);
    } // setupUi

    void retranslateUi(QMainWindow *EMGWidget)
    {
        EMGWidget->setWindowTitle(QCoreApplication::translate("EMGWidget", "EMG Viewer", nullptr));
        actionSave->setText(QCoreApplication::translate("EMGWidget", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("EMGWidget", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("EMGWidget", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("EMGWidget", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionClear_plot->setText(QCoreApplication::translate("EMGWidget", "Clear plot", nullptr));
        actionPlot_color->setText(QCoreApplication::translate("EMGWidget", "Plot color", nullptr));
        actionDevice_info->setText(QCoreApplication::translate("EMGWidget", "Device info", nullptr));
        sensorNumber->setText(QCoreApplication::translate("EMGWidget", "Number of sensors", nullptr));
        actionClear_log->setText(QCoreApplication::translate("EMGWidget", "Clear log", nullptr));
        actionClear_all->setText(QCoreApplication::translate("EMGWidget", "Clear all", nullptr));
        btn_ConnectDisconnect->setText(QCoreApplication::translate("EMGWidget", "Connect", nullptr));
        menuFile->setTitle(QCoreApplication::translate("EMGWidget", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("EMGWidget", "Edit", nullptr));
        menuAbout->setTitle(QCoreApplication::translate("EMGWidget", "About", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EMGWidget: public Ui_EMGWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMGWIDGET_H

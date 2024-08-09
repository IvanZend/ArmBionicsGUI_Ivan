/********************************************************************************
** Form generated from reading UI file 'emgwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMGWIDGET_H
#define UI_EMGWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_EMGWidget
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_ConnectDisconnect;
    QComboBox *cb_COMP;
    QCustomPlot *customPlot;

    void setupUi(QMainWindow *EMGWidget)
    {
        if (EMGWidget->objectName().isEmpty())
            EMGWidget->setObjectName("EMGWidget");
        EMGWidget->resize(800, 600);
        centralwidget = new QWidget(EMGWidget);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 10, 291, 41));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btn_ConnectDisconnect = new QPushButton(layoutWidget);
        btn_ConnectDisconnect->setObjectName("btn_ConnectDisconnect");

        horizontalLayout->addWidget(btn_ConnectDisconnect);

        cb_COMP = new QComboBox(layoutWidget);
        cb_COMP->setObjectName("cb_COMP");
        cb_COMP->setEnabled(true);
        cb_COMP->setMinimumSize(QSize(0, 0));
        cb_COMP->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(cb_COMP);

        customPlot = new QCustomPlot(centralwidget);
        customPlot->setObjectName("customPlot");
        customPlot->setGeometry(QRect(10, 80, 780, 520));
        EMGWidget->setCentralWidget(centralwidget);

        retranslateUi(EMGWidget);

        QMetaObject::connectSlotsByName(EMGWidget);
    } // setupUi

    void retranslateUi(QMainWindow *EMGWidget)
    {
        EMGWidget->setWindowTitle(QCoreApplication::translate("EMGWidget", "EMG Viewer", nullptr));
        btn_ConnectDisconnect->setText(QCoreApplication::translate("EMGWidget", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EMGWidget: public Ui_EMGWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMGWIDGET_H

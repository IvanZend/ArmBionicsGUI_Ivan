#ifndef EMGWIDGET_H
#define EMGWIDGET_H

#include "QtWidgets/qtextbrowser.h"
#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class EMGWidget; }
QT_END_NAMESPACE

class EMGWidget : public QMainWindow

{
    Q_OBJECT

public:
    EMGWidget(QWidget *parent = nullptr);
    ~EMGWidget();

private slots:
    void on_btn_ConnectDisconnect_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

public slots:
    void read_data(void);

private:
    Ui::EMGWidget *ui;
    bool connect_status = false;
    QSerialPort m_serial;
    void refreshGraph(void);
    void plotEMGGraph(void);
    QTextBrowser *logViewer;
    qint32 QByteArrayToInt(const QByteArray& bytes);
    void saveDataToFile(const QString& filename);
};

#endif // EMGWIDGET_H

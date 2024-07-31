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

    void on_actionClear_triggered();

    void on_actionPlot_color_triggered();

    void on_actionDevice_info_triggered();

public slots:
    void read_data(void);

private:
    Ui::EMGWidget *ui;
    bool connect_status = false;
    QColor plot_color = QColor(0,0,0);
    QSerialPort m_serial;
    QTextBrowser *logViewer;
    QString deviceID;

    void refreshGraph(void);
    void plotEMGGraph(void);
    void updateGraph(void);
    void saveDataToFile(const QString& filename);
    void loadDataFromFile(const QString& filename);

    qint32 QByteArrayToInt(const QByteArray& bytes);
};

#endif // EMGWIDGET_H

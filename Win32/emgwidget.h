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
    void handleSerialPortError(QSerialPort::SerialPortError error);

    void on_btn_ConnectDisconnect_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionClear_triggered();

    void on_actionPlot_color_triggered();

    void on_actionDevice_info_triggered();

    void on_sensorNumber_triggered();

public slots:
    void read_data(void);

private:
    Ui::EMGWidget *ui;
    bool connect_status = false;
    QSerialPort m_serial;
    QTextBrowser *logViewer;
    QString deviceID;
    quint8 num_emg = 8;
    QVector<QList<double>> emg_data = QVector<QList<double>>(num_emg);
    QByteArray buffer;
    bool auto_num = true;

    void portConfig(QSerialPort::BaudRate baudRate = QSerialPort::Baud115200, QSerialPort::DataBits dataBits = QSerialPort::Data8,
                    QSerialPort::Parity parity = QSerialPort::NoParity, QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    void portConnect(void);
    void portDisconnect(void);
    void refreshGraph(void);
    void plotEMGGraph(void);
    void updateGraph(void);
    void saveDataToFile(const QString& filename);
    void loadDataFromFile(const QString& filename);

    qint32 QByteArrayToInt(const QByteArray& bytes);
};

#endif // EMGWIDGET_H

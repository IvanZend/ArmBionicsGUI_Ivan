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

    void on_btn_ConnectDisconnect_clicked(void);

    void on_actionSave_triggered(void);

    void on_actionOpen_triggered(void);

    void on_actionClear_triggered(void);

    void on_actionPlot_color_triggered(void);

    void on_actionDevice_info_triggered(void);

    void on_sensorNumber_triggered(void);

public slots:
    void read_data(void);

private:
    Ui::EMGWidget *ui;

    bool connect_status = false;
    QSerialPort m_serial; // Serial port class to setup the COM Port

    QTextBrowser *logViewer; // To log data

    quint16 updateIntervalMs = 100; // Graph update of 100ms by default
    quint8 num_emg = 8; // Number of EMG sensors. Default 8
    bool auto_num = true; // Automatically count number of EMG sensors. Turns false if set manually
    QByteArray buffer; // Buffer to read data
    QVector<QList<double>> emg_data = QVector<QList<double>>(num_emg);

    // Device attributes
    QString deviceID = "none";
    quint8 batteryStatus = 0;
    bool motorStatus = false;

    void portConfig(QSerialPort::BaudRate baudRate = QSerialPort::Baud115200, QSerialPort::DataBits dataBits = QSerialPort::Data8,
                    QSerialPort::Parity parity = QSerialPort::NoParity, QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                    QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    void portConnect(void);
    void portDisconnect(void);
    void refreshGraph(void);
    void plotEMGGraph(void);
    void updateGraph(void);
    void updateDeviceInfo(void);
    void saveDataToFile(const QString& filename);
    void loadDataFromFile(const QString& filename);
    void setUpdateInterval(quint8 intervalMs);
    qint32 QByteArrayToInt(const QByteArray& bytes);
};

#endif // EMGWIDGET_H

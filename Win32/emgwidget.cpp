#include "emgwidget.h"
#include "./ui_emgwidget.h"
#include "logger.h"
#include <QDateTime>
#include <QResizeEvent>
#include <QRandomGenerator>
#include <QColorDialog>
#include <QInputDialog>
#include "definitions.h"

const qint16 SECONDS_SHOW_ON_GRAPH = 50;  // Display N seconds on the graph
QList<double> time_axis;
QList<QString> time_axis_string; // To save the data and for displaying purposes
static qint64 voltage_data_idx = 0;   // Used for x-axis range setting
static qint64 startTime;

EMGWidget::EMGWidget(QWidget *parent) : QMainWindow(parent) , ui(new Ui::EMGWidget)
{
    ui->setupUi(this);
    // Initialize the log viewer
    logToModel(ui->textBrowser->document());

    qDebug() << "Detecting Available Serial Ports";

    // Find available ports and give the option to select them
    QList<QSerialPortInfo> serial_port_infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port_info : serial_port_infos)
    {
        qDebug() << "Port:" << port_info.portName();
        // Add these found com ports to the combo box
        ui->cb_COMP->addItem(port_info.portName());
    }

    // Plot the EMG graph
    plotEMGGraph();
}

EMGWidget::~EMGWidget()
{
    // Close the serial port if it's open
    if(m_serial.isOpen())
    {
        m_serial.close();
    }

    delete ui;
}

void EMGWidget::portConfig(QSerialPort::BaudRate baudRate, QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
                           QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl)
{
    m_serial.setBaudRate(baudRate); // Set Baud rate (default 115200)
    m_serial.setDataBits(dataBits); // Set data bits (default 8)
    m_serial.setParity(parity); // Set parity (default none)
    m_serial.setStopBits(stopBits); // Set stop bits (default one stop)
    m_serial.setFlowControl(flowControl); // Set flow control (default none)
}

void EMGWidget::portConnect(void)
{
    qDebug() << "Serial Port Opened Successfully";
    m_serial.write("Hello World from Qt\r\n");
    connect_status = true;
    ui->btn_ConnectDisconnect->setText("Disconnect");

    // Disable the combo box
    ui->cb_COMP->setEnabled(false);

    // Connect signal and slots
    connect(&m_serial, SIGNAL(readyRead()), this, SLOT(read_data()));
    connect(&m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(handleSerialPortError(QSerialPort::SerialPortError)));
}

void EMGWidget::portDisconnect(void)
{
    qInfo() << "Disconnecting...";

    // Close the serial port
    disconnect(&m_serial, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(handleSerialPortError(QSerialPort::SerialPortError)));
    m_serial.close();
    connect_status = false;
    ui->btn_ConnectDisconnect->setText("Connect");

    // Enable the combo box
    ui->cb_COMP->setEnabled(true);
}

void EMGWidget::handleSerialPortError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qWarning() << "Error:" << m_serial.errorString();
        portDisconnect();
        QMessageBox::critical(this, tr("Critical Error"), m_serial.errorString());
    } else {
        qWarning() << "Serial port error:" << m_serial.errorString();
    }
}

void EMGWidget::read_data()
{
    if (!m_serial.isOpen()) {
        qWarning() << "Serial port not open. Cannot read data.";
        return;
    }

    buffer.append(m_serial.readAll());

    while (buffer.size() >= PACKET_SIZE)
    {
        if (buffer.left(KEYWORD_SIZE) == PACKET_KEYWORD)
        {
            QByteArray packet = buffer.left(PACKET_SIZE);

            // Count occurrences of EMG handles in the packet
            if(auto_num){
                quint8 countE = 0;
                for (char ch : packet)
                {
                    if (ch == EMG_HANDLE)
                    {
                        ++countE;
                    }
                }

                // Update num_emg based on count of 'e'
                if (countE != num_emg)
                {
                    num_emg = countE;
                    emg_data.resize(num_emg);
                    for (quint8 i = 0; i < num_emg; ++i)
                    {
                        if (i >= emg_data.size())
                        {
                            emg_data.append(QList<double>());
                        }
                    }
                }
            }

            buffer.remove(0, PACKET_SIZE);

            // Recover device ID (keyword, first 4 bytes) TEMPORARY, TO CHANGE LATER
            deviceID = QString::fromUtf8(packet.left(KEYWORD_SIZE));

            // To be changed later, bytes 4 to 7
            // deviceID = QString::fromUtf8(packet.mid(DEVICE_ID_START, DEVICE_ID_SIZE));

            // Recover battery and motor status


            // Find EMG start keywords
            if (packet[KEYWORD_SIZE] == EMG_HANDLE && packet[KEYWORD_SIZE + EMG_VALUE_SIZE + HANDLE_SIZE] == EMG_HANDLE)
            {
                double now = QDateTime::currentMSecsSinceEpoch();
                time_axis.append(now / 1000.0);  // Convert to seconds for plotting
                time_axis_string.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));

                QStringList emg_values;

                for (quint8 i = 0; i < num_emg; i++)
                {
                    QByteArray emg_bytes = packet.mid(KEYWORD_SIZE + i * EMG_VALUE_SIZE + (i + 1) * HANDLE_SIZE, EMG_VALUE_SIZE);
                    int emg = VOLTAGE_COEFFICIENT*QByteArrayToInt(emg_bytes);
                    emg_data[i].append(emg);
                    emg_values << QString("EMG%1: %2").arg(i + 1).arg(emg);
                }

                qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "\t" << emg_values.join(", ");
            }
            else
            {
                buffer.remove(0, 1);
            }
            updateDeviceInfo();
        }
        else
        {
            buffer.remove(0, 1);
        }
    }
}

qint32 EMGWidget::QByteArrayToInt(const QByteArray& bytes)
{
    // Function to convert 4-byte QByteArray to integer

    // Ensure the byte array represents a valid ASCII number
    QString str = QString::fromUtf8(bytes); // Convert bytes to QString (UTF-8)
    bool ok;
    int number = str.toInt(&ok); // Convert QString to integer
    return ok ? number : 0; // Return 0 if conversion fails
}

void EMGWidget::plotEMGGraph(void)
{
    // Clear graphs
    ui->customPlot->clearGraphs();

    // Add graph for each EMG sensor
    for (quint8 i = 0; i < num_emg; i++)
    {
        ui->customPlot->addGraph();

        // Set color for each graph
        QColor color;
        color.setHsv(360 / (i + 1), 255, 255);

        ui->customPlot->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph(i)->setPen(color);
        ui->customPlot->graph(i)->setBrush(Qt::NoBrush);
    }

    // Add axis labels
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Voltage");

    // Add x-axis ticks (time)
    QSharedPointer<QCPAxisTickerDateTime> date_time_ticker(new QCPAxisTickerDateTime);
    date_time_ticker->setDateTimeFormat("hh:mm:ss.zzz");
    ui->customPlot->xAxis->setTicker(date_time_ticker);

    // Allow zooming in/out from axes and dragging
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // Add title
    QCPTextElement *title = new QCPTextElement(ui->customPlot);
    title->setText("EMG Signal Real-Time Plot");
    title->setFont(QFont("Helvetica", 12, QFont::Bold));
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, title);

    // Refresh graph
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EMGWidget::refreshGraph);
    timer->start(updateIntervalMs);  // Use the update interval
}

void EMGWidget::setUpdateInterval(quint8 intervalMs)
{
    if (intervalMs > 0)
    {
        updateIntervalMs = intervalMs;

        // If the timer is running, restart it with the new interval
        QTimer *timer = findChild<QTimer*>();
        if (timer)
        {
            timer->stop();
            timer->start(updateIntervalMs);
        }

        qDebug() << "Update interval set to:" << updateIntervalMs << "ms";
    }
    else
    {
        qWarning() << "Invalid update interval:" << intervalMs;
    }
}

void EMGWidget::updateDeviceInfo(void)
{
    // Create a string with the device information
    QString infoText = QString("Device ID: %1\nBattery: %2%\nMotor: %3")
                           .arg(deviceID)
                           .arg(batteryStatus)
                           .arg(motorStatus ? "On" : "Off");

    // Find or create the text element for displaying the information
    QCPTextElement *infoElement = nullptr;
    if (ui->customPlot->plotLayout()->elementCount() > 1)
    {
        infoElement = qobject_cast<QCPTextElement*>(ui->customPlot->plotLayout()->element(1, 0));
    }

    if (!infoElement)
    {
        infoElement = new QCPTextElement(ui->customPlot, infoText, QFont("Helvetica", 10));
        ui->customPlot->plotLayout()->insertRow(1); // Add a new row for the text
        ui->customPlot->plotLayout()->addElement(1, 0, infoElement);
    }

    // Update the text with the latest information
    infoElement->setText(infoText);

    // Refresh the plot
    ui->customPlot->replot();
}


void EMGWidget::refreshGraph(void)
{
    double now = QDateTime::currentMSecsSinceEpoch() / 1000.0;  // Convert to seconds
    if (connect_status)
    {
        for (quint8 i = 0; i < num_emg; i++)
        {
            ui->customPlot->graph(i)->setData(time_axis, emg_data[i]);
        }

        if (((qint64)(now * 1000) - startTime) > SECONDS_SHOW_ON_GRAPH * 1000)
        {
            ui->customPlot->xAxis->setRange(now, SECONDS_SHOW_ON_GRAPH, Qt::AlignRight);
        }
        ui->customPlot->replot();
    }
}

void EMGWidget::saveDataToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    out << "Time";
    for (int i = 0; i < num_emg; ++i)
    {
        out << (filename.endsWith(".csv", Qt::CaseInsensitive) ? "," : "\t") << "EMG" << (i + 1);
    }
    out << "\n";

    for (int i = 0; i < time_axis.size(); ++i)
    {
        out << time_axis_string[i];
        for (int j = 0; j < num_emg; ++j)
        {
            out << (filename.endsWith(".csv", Qt::CaseInsensitive) ? "," : "\t") << emg_data[j][i];
        }
        out << "\n";
    }

    file.close();
    qInfo() << "Data saved to" << filename;
}

void EMGWidget::loadDataFromFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for reading:" << file.errorString();
        return;
    }

    QTextStream in(&file);

    // Clear previous data
    time_axis.clear();
    time_axis_string.clear();
    for (int i = 0; i < num_emg; ++i) {
        emg_data[i].clear();
    }

    QString line;

    // Read and parse data from file
    QString delimiter = filename.endsWith(".csv", Qt::CaseInsensitive) ? "," : "\t";

    // Skip the header line
    in.readLine();

    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList fields = line.split(delimiter);
        if (fields.size() >= (num_emg + 1))
        {
            bool timeOk;
            double time = QDateTime::fromString(fields[0], "hh:mm:ss.zzz").toMSecsSinceEpoch() / 1000.0;
            if (time != -1)  // Check if time conversion was successful
            {
                time_axis.append(time);
                time_axis_string.append(fields[0]);

                bool allEmgOk = true;
                QVector<double> emg_values(num_emg);
                for (int i = 0; i < num_emg; ++i) {
                    bool emgOk;
                    emg_values[i] = fields[i + 1].toDouble(&emgOk);
                    allEmgOk = allEmgOk && emgOk;
                }

                if (allEmgOk) {
                    for (int i = 0; i < num_emg; ++i) {
                        emg_data[i].append(emg_values[i]);
                    }
                }
            }
        }
    }

    file.close();

    // Update the graph with the new data
    updateGraph();
}

void EMGWidget::updateGraph()
{
    // Ensure the plot is cleared before adding new data
    ui->customPlot->clearGraphs();

    // Add a graph for each EMG channel and set the data
    for (int i = 0; i < num_emg; ++i)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setData(time_axis, emg_data[i]);

        // Set different colors for each channel, for example:
        QColor color;
        color.setHsv(360/(i+1), 255, 255); // Saturation and value set to max (255) for full color

        ui->customPlot->graph(i)->setPen(QPen(color));
    }

    // Adjust the axes ranges based on the new data
    if (!time_axis.isEmpty())
    {
        ui->customPlot->xAxis->setRange(time_axis.first(), time_axis.last());

        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::min();
        for (const auto& channelData : emg_data)
        {
            double channelMin = *std::min_element(channelData.begin(), channelData.end());
            double channelMax = *std::max_element(channelData.begin(), channelData.end());
            if (channelMin < minY) minY = channelMin;
            if (channelMax > maxY) maxY = channelMax;
        }
        ui->customPlot->yAxis->setRange(minY, maxY);
    }

    // Replot the graph
    ui->customPlot->replot();
}

void EMGWidget::on_btn_ConnectDisconnect_clicked(void)
{
    double now;
    // Connect if not-connected, disconnect if connected.
    if(!connect_status)
    {
        qInfo() << "Connecting...";

        // Port configuration
        portConfig();

        // Select the COM Port from Combo Box
        m_serial.setPortName(ui->cb_COMP->currentText());
        if(m_serial.open(QIODevice::ReadWrite))
        {
            portConnect();

            startTime = QDateTime::currentSecsSinceEpoch();
            now = startTime;
            // set axes ranges, so we see all data:
            ui->customPlot->xAxis->setRange(now, now+SECONDS_SHOW_ON_GRAPH*1000);
            ui->customPlot->yAxis->setRange(0, 60);
        }
        else
        {
            qDebug() << "Unable to open the Selected Serial Port" << m_serial.error();
            QMessageBox::critical(this, "Error", "Unable to open the selected serial port. Please check if the device is connected and the port is available.");
        }
    }
    else
    {
        portDisconnect();
    }
}

void EMGWidget::on_actionDevice_info_triggered()
{
    if(deviceID.isEmpty()){
        QMessageBox::information(this, "Device Info", "Device ID: Unavailable");
    }
    else{
    QMessageBox::information(this, "Device Info", "Device ID: " + deviceID);
    }
}

void EMGWidget::on_actionSave_triggered()
{
    if(connect_status){
    portDisconnect();
    }
    QString filename = QFileDialog::getSaveFileName(this, "Save Data", "", "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)");
    if (!filename.isEmpty())
    {
        // Ensure the file has the correct extension
        if (!filename.endsWith(".txt", Qt::CaseInsensitive) && !filename.endsWith(".csv", Qt::CaseInsensitive))
        {
            // Default to .txt if no extension is provided
            filename.append(".txt");
        }
        saveDataToFile(filename);
    }
}

void EMGWidget::on_actionOpen_triggered(void)
{
    if(connect_status){
        portDisconnect();
    }

    QString filename = QFileDialog::getOpenFileName(this, "Open Data", "", "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)");
    if (!filename.isEmpty())
    {
        loadDataFromFile(filename);
    }
}

void EMGWidget::on_sensorNumber_triggered()
{
    auto_num = false;

    // Do not allow changes in connected state
    portDisconnect();

    // Clear data before changing dimensions
    on_actionClear_triggered();

    bool ok;
    quint8 numSensors = QInputDialog::getInt(this, tr("Set Number of Sensors"),
                                             tr("Number of Sensors:"), num_emg, 1, 100, 1, &ok);

    if (numSensors != num_emg)
    {
        num_emg = numSensors;

        // Clear previous data
        emg_data.resize(num_emg);
        for (int i = 0; i < num_emg; ++i) {
            emg_data[i].clear();
        }
        plotEMGGraph();
    }
}

void EMGWidget::on_actionPlot_color_triggered()
{
    // Create a dialog to select the graph to change the color
    bool ok;
    QStringList graphNames;
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        graphNames << QString("EMG %1").arg(i + 1);
    }

    QString selectedGraphName = QInputDialog::getItem(this, "Select Graph", "Choose the graph to change color:", graphNames, 0, false, &ok);
    if (!ok || selectedGraphName.isEmpty()) {
        qDebug() << "Color change canceled.";
        return;
    }

    // Determine the index of the selected graph
    int graphIndex = graphNames.indexOf(selectedGraphName);

    // Open a color dialog and get the selected color
    QColor newColor = QColorDialog::getColor(Qt::black, this, "Select Plot Color"); // Default color is black

    // Check if the user clicked "OK" and a valid color was selected
    if (newColor.isValid())
    {
        // Update the plot color for the selected graph
        if (graphIndex >= 0 && graphIndex < ui->customPlot->graphCount())
        {
            ui->customPlot->graph(graphIndex)->setPen(QPen(newColor));
            ui->customPlot->replot(); // Refresh the plot
            qDebug() << QString("Graph %1 color changed to: %2").arg(graphIndex + 1).arg(newColor.name()); // Log the new color
        }
        else
        {
            qWarning() << "Invalid graph index:" << graphIndex;
        }
    }
    else
    {
        qDebug() << "Color change canceled or invalid color selected.";
    }
}

void EMGWidget::on_actionClear_triggered()
{
    // Clear all graphs from the plot
    ui->customPlot->clearGraphs();

    // Clear the data structures
    time_axis.clear();
    time_axis_string.clear();
    for (int i = 0; i < num_emg; i++)
    {
        emg_data[i].clear();  // Clear each QList<double> in the QVector
    }

    // Re-add the graphs for each EMG channel
    for (int i = 0; i < num_emg; i++)
    {
        QColor color;
        color.setHsv(360/(i+1), 255, 255); // Saturation and value set to max (255) for full color

        ui->customPlot->addGraph(); // Add a new graph for each EMG channel
        ui->customPlot->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph(i)->setPen(QPen(color));
        ui->customPlot->graph(i)->setBrush(Qt::NoBrush);
    }

    // Update the graph with the cleared data
    ui->customPlot->replot();

    qDebug() << "Plot data cleared.";
}

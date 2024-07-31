#include "emgwidget.h"
#include "./ui_emgwidget.h"
#include "logger.h"
#include <QDateTime>
#include <QResizeEvent>
#include "definitions.h"

const qint16 SECONDS_SHOW_ON_GRAPH = 20;  // Display 120 seconds on the graph
QList<double> time_axis;
QList<QString> time_axis_string; // To save the data and for displaying purposes
QList<double> voltage_axis;
static qint64 voltage_data_idx = 0;   // Used for x-axis range setting
static qint64 startTime;


EMGWidget::EMGWidget(QWidget *parent) : QMainWindow(parent) , ui(new Ui::EMGWidget)
{
    ui->setupUi(this);
    connect(ui->actionSave, &QAction::triggered, this, &EMGWidget::on_actionSave_triggered);

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

void EMGWidget::on_btn_ConnectDisconnect_clicked(void)
{
    double now;
    // if false, we have to connect, else disconnect
    if(connect_status == false)
    {
        qInfo() << "Connecting...";

        // Port configuration
        m_serial.setBaudRate(QSerialPort::Baud115200);
        m_serial.setDataBits(QSerialPort::Data8);
        m_serial.setParity(QSerialPort::NoParity);
        m_serial.setStopBits(QSerialPort::OneStop);
        m_serial.setFlowControl(QSerialPort::NoFlowControl);

        // Select the COM Port from Combo Box
        m_serial.setPortName(ui->cb_COMP->currentText());;
        if(m_serial.open(QIODevice::ReadWrite))
        {
            qDebug() << "Serial Port Opened Successfully";
            m_serial.write("Hello World from Qt\r\n");
            connect_status = true;
            ui->btn_ConnectDisconnect->setText("Disconnect");
            // disable the combo box
            ui->cb_COMP->setEnabled(false);
            // Connect Signal and Slots
            connect(&m_serial, SIGNAL(readyRead()), this, SLOT(read_data()));

            startTime = QDateTime::currentSecsSinceEpoch();
            now = startTime;
            // set axes ranges, so we see all data:
            ui->customPlot->xAxis->setRange(now, now+SECONDS_SHOW_ON_GRAPH);
            ui->customPlot->yAxis->setRange(0, 60);
        }
        else
        {
            qDebug() << "Unable to open the Selected Serial Port" << m_serial.error();
        }
    }
    else
    {
        qInfo() << "Disconnecting...";
        // close the serial port
        m_serial.close();
        connect_status = false;
        ui->btn_ConnectDisconnect->setText("Connect");
        // Enable the combo box
        ui->cb_COMP->setEnabled(true);
    }
}

void EMGWidget::read_data()
{
    static QByteArray buffer; // To accumulate incoming data

    // Read all available bytes
    buffer.append(m_serial.readAll());

    // Process the buffer if it has enough data
    while (buffer.size() >= PACKET_SIZE)
    {
        // Check for the start keyword
        if (buffer.left(KEYWORD_SIZE) == PACKET_KEYWORD)
        {
            // Extract the packet
            QByteArray packet = buffer.left(PACKET_SIZE);
            buffer.remove(0, PACKET_SIZE);

            // Validate markers and extract EMG data
            if (packet[KEYWORD_SIZE] == EMG_START_CHAR && packet[KEYWORD_SIZE+EMG_VALUE_SIZE+EMG_START_SIZE] == EMG_START_CHAR)
            {
                // Extract the EMG data bytes
                QByteArray emg1_bytes = packet.mid(KEYWORD_SIZE+EMG_START_SIZE, EMG_VALUE_SIZE);
                QByteArray emg2_bytes = packet.mid(KEYWORD_SIZE+EMG_VALUE_SIZE+2*EMG_START_SIZE, EMG_VALUE_SIZE);

                // Convert bytes to integers (assuming little-endian)
                int emg1 = QByteArrayToInt(emg1_bytes);
                int emg2 = QByteArrayToInt(emg2_bytes);

                // Convert to voltage values or directly use as needed
                double now = QDateTime::currentSecsSinceEpoch();

                // Append data for plotting
                time_axis_string.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
                time_axis_string.append(QDateTime::currentDateTime().toString("hh:mm:ss"));
                voltage_axis.append(emg1);
                voltage_axis.append(emg2);
                time_axis.append(now);
                time_axis.append(now);

                // Debug output
                qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "\t" << "EMG1:" << emg1 << "\t" << "EMG2:" << emg2;
            }
            else
            {
                // Invalid packet format, discard the first byte
                buffer.remove(0, 1);
            }
        }
        else
        {
            // If the start keyword is not found, discard the first byte
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

void EMGWidget::plotEMGGraph(void){

    // Add graph
    ui->customPlot->addGraph();

    // Set axis labels
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Voltage");

    // Set plot line color (RGB)
    QColor color(40, 110, 255);

    // Set line, pen and brush styles
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph(0)->setPen(QPen(color.lighter(30)));
    ui->customPlot->graph(0)->setBrush(Qt::NoBrush);

    // Set time on the x-axis:
    QSharedPointer<QCPAxisTickerDateTime> date_time_ticker(new QCPAxisTickerDateTime);
    date_time_ticker->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTicker(date_time_ticker);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // first we create and prepare a text layout element:
    QCPTextElement *title = new QCPTextElement(ui->customPlot);
    title->setText("EMG Signal Real-Time Plot");
    title->setFont(QFont("Helvetica", 12, QFont::Bold));

    // then we add it to the main plot layout:
    // insert an empty row above the axis rect
    ui->customPlot->plotLayout()->insertRow(0);

    // place the title in the empty cell we've just created
    ui->customPlot->plotLayout()->addElement(0, 0, title);

    // Start Timer to refresh the graph
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EMGWidget::refreshGraph);

    // Start timer at 1 second
    timer->start(1000);

}

void EMGWidget::refreshGraph(void)
{
    double now = QDateTime::currentSecsSinceEpoch();
    if(connect_status)
    {
        ui->customPlot->graph()->setData(time_axis, voltage_axis);
        // if time has elapsed then only start shifting the graph
        if(((qint64)(now) - startTime) > SECONDS_SHOW_ON_GRAPH)
        {
            // If SECONDS_SHOW_GRAPH
            ui->customPlot->xAxis->setRange(now, SECONDS_SHOW_ON_GRAPH, Qt::AlignRight);
        }
        ui->customPlot->replot();
    }
}

void EMGWidget::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Data", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty())
    {
        saveDataToFile(filename);
    }
}

void EMGWidget::saveDataToFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    // Write header
    out << "Time,\t EMG1,\t EMG2\n";

    // Write data
    for (int i = 0; i < time_axis.size(); ++i)
    {
        out << time_axis_string[i] << ", " << voltage_axis[i] << ", " << voltage_axis[i + 1] << "\n";
        i++; // Increment to skip the next value because we're writing pairs
    }

    file.close();
    qInfo() << "Data saved to" << filename;
}

void EMGWidget::on_actionOpen_triggered(void)
{
    // Open a file dialog to select the file to open
    QString filename = QFileDialog::getOpenFileName(this, "Open Data", "", "Text Files (*.txt);;All Files (*)");
    if (!filename.isEmpty())
    {
        // Load and plot the data from the selected file
        loadDataFromFile(filename);
    }
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
    voltage_axis.clear();
    time_axis_string.clear();

    // Read and parse data from file
    QString line;
    // Skip the header line
    in.readLine();

    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList fields = line.split(", ");
        if (fields.size() >= 3)
        {
            bool timeOk, emg1Ok, emg2Ok;
            double time = QDateTime::fromString(fields[0], "hh:mm:ss").toSecsSinceEpoch();
            double emg1 = fields[1].toDouble(&emg1Ok);
            double emg2 = fields[2].toDouble(&emg2Ok);

            if (emg1Ok && emg2Ok)
            {
                time_axis.append(time);
                voltage_axis.append(emg1);
                voltage_axis.append(emg2);
                time_axis_string.append(fields[0]);
                time_axis_string.append(fields[0]); // Assuming each line has 2 EMG values
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

    // Re-add the graph and set the data
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(time_axis, voltage_axis);

    // Adjust the axes ranges based on the new data
    if (!time_axis.isEmpty())
    {
        ui->customPlot->xAxis->setRange(time_axis.first(), time_axis.last());
        ui->customPlot->yAxis->setRange(*std::min_element(voltage_axis.begin(), voltage_axis.end()),
                                        *std::max_element(voltage_axis.begin(), voltage_axis.end()));
    }

    // Replot the graph
    ui->customPlot->replot();
}

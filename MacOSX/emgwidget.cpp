#include "emgwidget.h"
#include "./ui_emgwidget.h"
#include "logger.h"
#include <QDateTime>
#include <QResizeEvent>
#include <QRandomGenerator>
#include <QColorDialog>
#include "definitions.h"

const qint16 SECONDS_SHOW_ON_GRAPH = 50;  // Display 120 seconds on the graph
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
    static QRandomGenerator randomGenerator1;
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

            deviceID = QString::fromUtf8(packet.left(4));

            // Validate markers and extract EMG data
            if (packet[KEYWORD_SIZE] == EMG_START_CHAR && packet[KEYWORD_SIZE+EMG_VALUE_SIZE+EMG_START_SIZE] == EMG_START_CHAR)
            {
                double now = QDateTime::currentSecsSinceEpoch();
                time_axis.append(now);
                time_axis_string.append(QDateTime::currentDateTime().toString("hh:mm:ss"));

                // For debug output
                QStringList emg_values;

                for(quint8 i = 0; i < num_emg; i++){
                // Extract the EMG data bytes
                QByteArray emg_bytes = packet.mid(KEYWORD_SIZE+i*EMG_VALUE_SIZE+(i+1)*EMG_START_SIZE, EMG_VALUE_SIZE);

                // Convert bytes to integers (assuming little-endian)
                int emg = QByteArrayToInt(emg_bytes);

                // TEMPORARY: using random values to generate data
                double rng_multiplier1 = randomGenerator1.bounded(10);

                // Append data for plotting
                emg_data[i].append(emg/ 1000);

                // Format debug output
                emg_values << QString("EMG%1: %2").arg(i+1).arg(emg);
                }

                // Debug output
                qDebug() << QDateTime::currentDateTime().toString("hh:mm:ss") << "\t" << emg_values.join(", ");
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
    for(quint8 i = 0; i < num_emg; i++){
        ui->customPlot->addGraph();

        QColor color;
        color.setHsv(360/(i+1), 255, 255); // Saturation and value set to max (255) for full color

        // Set line, pen and brush styles
        ui->customPlot->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->customPlot->graph(i)->setPen(color);
        ui->customPlot->graph(i)->setBrush(Qt::NoBrush);
    }

    // Set axis labels
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Voltage");

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
        for(quint8 i = 0 ; i < num_emg; i++){
        ui->customPlot->graph(i)->setData(time_axis, emg_data[i]);
        }

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
    out << "Time";
    for (int i = 0; i < num_emg; ++i) {
        out << ",\t EMG" << (i + 1);
    }
    out << "\n";

    // Write data
    for (int i = 0; i < time_axis.size(); ++i)
    {
        out << time_axis_string[i];
        for (int j = 0; j < num_emg; ++j) {
            out << ",\t" << emg_data[j][i];
        }
        out << "\n";
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
    time_axis_string.clear();
    for (int i = 0; i < num_emg; ++i) {
        emg_data[i].clear();
    }

    // Read and parse data from file
    QString line;
    // Skip the header line
    in.readLine();

    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList fields = line.split(",\t");
        if (fields.size() >= (num_emg + 1))
        {
            bool timeOk;
            double time = QDateTime::fromString(fields[0], "hh:mm:ss").toSecsSinceEpoch();
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

void EMGWidget::on_actionDevice_info_triggered()
{
    if(deviceID.isEmpty()){
        QMessageBox::information(this, "Device Info", "Device ID: Unavailable");
    }
    else{
    QMessageBox::information(this, "Device Info", "Device ID: " + deviceID);
    }
}


#include "arduino_usb.h"

ArduinoUsb::ArduinoUsb()
{
    usbPort = new QSerialPort(this);
    usbInfo = NULL;
    this->searchDevices();

    connect(usbPort, SIGNAL(readyRead()), this, SLOT(readUsbData()));
}

ArduinoUsb::~ArduinoUsb()
{
    if(usbPort->isOpen())
        usbPort->close();
}

void ArduinoUsb::searchDevices()
{
    availablePort = QSerialPortInfo::availablePorts();
    int nPorts = availablePort.size();
    emit this->updatePortsComboBox(nPorts);
}

void ArduinoUsb::portIndexChanged(int nIndex)
{
    if(usbPort->isOpen()) usbPort->close();
    QString QStrPortName = "NULL";
    if (nIndex > 0)
    {
        usbInfo = &availablePort.at(nIndex-1);
        QStrPortName = usbInfo->portName();
        usbPort->setPort(availablePort.at(nIndex-1));

        if(!usbPort->open(QIODevice::ReadWrite))
            qCritical() << "unable to open port";
    }
    else usbInfo = NULL;

    if (QStrPortName != "NULL")
        emit this->addTextToLogPTE("Connected to port: " + QStrPortName + "\n", LOG_USB);
}

void ArduinoUsb::sendDataToUsb(QString QStrMsg)
{
    if(usbPort->isOpen())
    {
        emit this->addTextToLogPTE(QStrMsg + "\n", LOG_USB_SENT);
        QStrMsg += "$"; //read serial port until $ sign

        usbPort->write(QStrMsg.toStdString().c_str());
        usbPort->waitForBytesWritten(-1);
    }
    else qCritical() << "USB port is closed";
}

void ArduinoUsb::readUsbData()
{
    QByteA_data = usbPort->readAll();

    QString QsQByteA(QByteA_data);
    QStrFullSerialMsg += QsQByteA;

    if (QStrFullSerialMsg.at(0) == '@' && QStrFullSerialMsg.at(QStrFullSerialMsg.size()-1) == '$')
    {
        QStrFullSerialMsg.remove('$');
        QStrFullSerialMsg.remove('@');

        QJsonDocument jsonDoc = QJsonDocument::fromJson(QStrFullSerialMsg.toUtf8());
        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            Sensors sensors = this->readSensors(QStrFullSerialMsg);
            emit this->updateSensorsInUI(sensors);
            emit this->msgFromUsbToChess(sensors);
        }

        QByteA_data.clear();
        QStrFullSerialMsg.clear();
    }
}

/*static*/ Sensors ArduinoUsb::readSensors(const QString& QStrMsg)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QStrMsg.toUtf8());
    QJsonObject jsonObj;
    if(!jsonDoc.isNull())
    {
        if (jsonDoc.isObject())
            jsonObj = jsonDoc.object();
        else qCritical() << "Document is not an object";
    }
    else qCritical() << "Invalid JSON: " << QStrMsg;

    Sensors sensors;
    sensors.bStopButton = jsonObj["stopBtn"].toInt();
    sensors.fDustDensity = jsonObj["dust"].toDouble();
    sensors.fCurrent = jsonObj["current"].toDouble();
    sensors.fTemp1 = jsonObj["temp1"].toDouble();
    sensors.fTemp2 = jsonObj["temp2"].toDouble();
    return sensors;
}


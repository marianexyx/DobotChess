#include "arduino_usb.h"

ArduinoUsb::ArduinoUsb()
{
    usbPort = new QSerialPort(this);
    usbInfo = NULL;
    this->searchDevices();

    connect(usbPort, SIGNAL(readyRead()), this, SLOT(readUsbData()));
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

    qInfo() << "QByteA_data = usbPort->readAll():" << QByteA_data;
    QString QsQByteA(QByteA_data);
    QStrFullSerialMsg += QsQByteA;

    if (QStrFullSerialMsg.at(0) == '@' && QStrFullSerialMsg.at(QStrFullSerialMsg.size()-1) == '$')
    {
        QStrFullSerialMsg.remove('$');
        QStrFullSerialMsg.remove('@');

        emit this->addTextToLogPTE(QStrFullSerialMsg + "\n", LOG_USB_RECEIVED);
        emit this->msgFromUsbToChess(QStrFullSerialMsg);

        QByteA_data.clear();
        QStrFullSerialMsg.clear();
    }
}

ArduinoUsb::~ArduinoUsb()
{
    if(usbPort->isOpen())
        usbPort->close();
}

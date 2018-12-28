#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H
#pragma once
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include "vars/log.h"

struct Sensors
{
    bool bStopButton;
    float fDustDensity;
    float fCurrent;
    float fTemp1;
    float fTemp2;

    QString getAsQStr()
    {
        return "bStopButton: " + QString::number(bStopButton)
                + ", fDustDensity: " + QString::number(fDustDensity) +
                ", fCurrent: " + QString::number(fCurrent) +
                ", temps: " + QString::number(fTemp1) + "/" + QString::number(fTemp2);
    }
};

//future: update names
class ArduinoUsb: public QObject
{
    Q_OBJECT

public:
    ArduinoUsb();
    ~ArduinoUsb();

    QList <QSerialPortInfo> availablePort;
    const QSerialPortInfo* usbInfo;
    QSerialPort* usbPort;
    QString QStrFullSerialMsg;

    static Sensors readSensors(const QString& QStrMsg);
    void sendDataToUsb(QString QStrMsg);
    void searchDevices();

private:
    QByteArray QByteA_data; //array for packets received from usb
    void receive();

public slots:
   void portIndexChanged(int nIndex);

private slots:
    void readUsbData();

signals:
    void addTextToLogPTE(QString, LOG);
    void updatePortsComboBox(int);
    void updateSensorsInUI(Sensors);
    void msgFromUsbToChess(Sensors);
};

#endif // ARDUINOUSB_H

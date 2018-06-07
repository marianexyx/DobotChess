#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H

#pragma once
#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "vars/log.h"
#include "vars/basic_vars.h"

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

    void sendDataToUsb(QString QStrMsg);
    void searchDevices();

private:
    QByteArray QByteA_data; //array for packets received from usb

    void receive();

public slots:
   void portIndexChanged(int nIndex);

private slots:
    //void portIndexChanged(int index);
    void readUsbData();

signals:
    void addTextToLogPTE(QString, LOG);
    void updatePortsComboBox(int);
    void msgFromUsbToChess(QString);
};

#endif // ARDUINOUSB_H

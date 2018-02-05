#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H

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
    QByteArray QByteA_data; //tablica niezorganizowanych danych przypływających z usb

    void receive(); //odbieranie wiadomości z usb

public slots:
   void portIndexChanged(int nIndex); //zmiana/wybór portu

private slots:
    //void portIndexChanged(int index); //zmiana/wybór portu
    void readUsbData(); //zczytywanie danych lecących asynchronicznie z usb

signals:
    void addTextToLogPTE(QString, LOG);
    void updatePortsComboBox(int);
    void msgFromUsbToChess(QString);
};

#endif // ARDUINOUSB_H

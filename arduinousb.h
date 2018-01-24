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
    QString QsFullSerialMsg;

    void sendDataToUsb(QString QsMsg);
    void searchDevices();
    void manageMsgFromUsb(QString QStrMsg);

private:
    QByteArray QByteA_data; //tablica niezorganizowanych danych przypływających z usb

    void receive(); //odbieranie wiadomości z usb

public slots:
   void portIndexChanged(int index); //zmiana/wybór portu

private slots:
    //void portIndexChanged(int index); //zmiana/wybór portu
    void readUsbData(); //zczytywanie danych lecących asynchronicznie z usb

    //TODO: te sygnały to rympał w ogarnianiu kodu. działają na klasy pod którymi nie są, co...
    //...powoduje chaos w ogarnianiu co skąd i kiedy leci
    //todo: sygnały wiedzą że klasa ma związek z szachami. usb powinno tylko przepychać dane...
    //...w 2 strony
signals:
    void addTextToLogPTE(QString, LOG);
    void updatePortsComboBox(int);
    void sigRestoreGameToInitialState(); //future: nazwa gryzie się z funkcja robiącą...
    //...w sumie coś innego
    void AIEnemySend(QString);
    void TcpQueueMsg(int, QString); //todo: usb nie powinno mieć dostępu bokiem do tcp
};

#endif // ARDUINOUSB_H

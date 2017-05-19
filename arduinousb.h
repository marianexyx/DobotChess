#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H

#include <QObject>
#include <QDebug>
#include <QSerialPort> //obsługa portu
#include <QSerialPortInfo> //dostarczanie informacji o porcie
#include "vars/log.h"
#include "vars/basic_vars.h"

class ArduinoUsb: public QObject
{
    Q_OBJECT

public:
    ArduinoUsb();

    ~ArduinoUsb();

    QList <QSerialPortInfo> availablePort;     // lista portów pod którymi są urządzenia
    const QSerialPortInfo *usbInfo;                // obecnie wybrany serial port
    QSerialPort *usbPort;                        // obecnie otwarty port
    QString QsFullSerialMsg;

    void sendDataToUsb(QString QsMsg); //wysyłanie wiadomośći na usb
    void searchDevices();
    void ManageMsgFromUsb(QString QsUsbMsg);

private:
    QByteArray QByteA_data; // tablica niezorganizowanych danych przypływających z usb

    void receive(); //odbieranie wiadomości z usb

public slots:
   void portIndexChanged(int index); //zmiana/wybór portu

private slots:
    //void portIndexChanged(int index); //zmiana/wybór portu
    void readUsbData(); //zczytywanie danych lecących asynchronicznie z usb

    //TODO: te sygnały to rympał w ogarnianiu kodu. działają na klasy pod którymi nie są, co...
    //...powoduje chaos w ogarnianiu co skąd i kiedy leci
signals:
    void addTextToConsole(QString, LOG);
    void updatePortsComboBox(int);
    void reset();
    void AIEnemySend(QString);
    void TcpQueueMsg(int, QString); //zakolejkuj żądanie do wykonania na tcp
};

#endif // ARDUINOUSB_H

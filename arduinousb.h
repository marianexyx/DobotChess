#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H

#include <QSerialPort> //obsługa portu
#include <QSerialPortInfo> //dostarczanie informacji o porcie

class ArduinoUsb
{
public:
    ArduinoUsb();

    ~ArduinoUsb();

private:
    QList <QSerialPortInfo> available_port;     // lista portów pod którymi są urządzenia
    const QSerialPortInfo *info;                // obecnie wybrany serial port
    QSerialPort usb_port;                       // obecnie otwarty port
    //QSerialPort *usb_port;      //todo: ???
    void searchDevices();
    void sendDataToUsb(QString QS_msg, bool sender=false); //wysyłanie wiadomośći na usb
    void receive(); //odbieranie wiadomości z usb

private slots:
    void on_port_currentIndexChanged(int index); //zmiana/wybór portu
    void refresh();
    void on_commandLine_returnPressed(); //reakcja na wciśnięcie entera w programie
    void readUsbData(); //sczytywanie danych lecących asynchronicznie z usb
};

#endif // ARDUINOUSB_H

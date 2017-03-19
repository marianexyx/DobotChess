#ifndef ARDUINOUSB_H
#define ARDUINOUSB_H

#include <QObject>
#include <QDebug>
#include <QSerialPort> //obsługa portu
#include <QSerialPortInfo> //dostarczanie informacji o porcie

//TODO: przemysleć co private, co public

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


private:
    //QList <QSerialPortInfo> availablePort;     // lista portów pod którymi są urządzenia
    //const QSerialPortInfo *usbInfo;                // obecnie wybrany serial port
    //QSerialPort usbPort;                       // obecnie otwarty port //todo: ???
    //QSerialPort *usbPort;                        // obecnie otwarty port. test as public
    //void sendDataToUsb(QString QsMsg, bool sender=false); //wysyłanie wiadomośći na usb
    QByteArray QByteA_data; // tablica niezorganizowanych danych przypływających z usb

    void receive(); //odbieranie wiadomości z usb
    void ManageMsgFromUsb(QString QsUsbMsg);

public slots:
   void portIndexChanged(int index); //zmiana/wybór portu

private slots:
    //void portIndexChanged(int index); //zmiana/wybór portu
    void readUsbData(); //sczytywanie danych lecących asynchronicznie z usb

    //TODO: te sygnały to rympał w ogarnianiu kodu. działają na klasy pod którymi nie są, co...
    //...powoduje chaos w ogarnianiu co skąd i kiedy leci
signals:
    void addTextToUsbConsole(QString, char);
    void updatePortsComboBox(int);
    void AIEnemyStart();
    void AIEnemySend(QString);
    void TcpQueueMsg(QString); //zakolejkuj żądanie do wykonania na tcp
};

#endif // ARDUINOUSB_H

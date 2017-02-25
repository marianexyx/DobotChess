#include "arduinousb.h"

ArduinoUsb::ArduinoUsb()
{
    usbPort = new QSerialPort(this);
    usbInfo = NULL; //wartośc wskażnika obecnie wybranego portu ustgawiamy na pustą wartość
    this->searchDevices(); //wyszukujemy obecnie podłączone urządzenia usb

    connect(usbPort, SIGNAL(readyRead()), this, SLOT(readUsbData()));
}

//aktualizowanie listy z urządzeniami
//tutaj wyszukujemy obecnie podłączone urządzenia usb i „wrzucamy” je do wcześniej przygotowanej listy
void ArduinoUsb::searchDevices()
{
    availablePort = QSerialPortInfo::availablePorts(); //dodanie ich do listy
    int nPorts = availablePort.size(); //ilość portów (wielkość listy)
    emit this->updatePortsComboBox(nPorts);
}

//Slot jest aktywowany po zmianie wartości przez użytkownika w combo box’ie. Ustawia on wskaźnik
//na nowy obiekt (lub nic) i wyświetla odpowiednią informację w pasku statusu
void ArduinoUsb::portIndexChanged(int nPortIndex) //zmiana/wybór portu
{ //bodajze jest ustawianie połączenie z portem automatycznie jak tylko zostanie on wybrany
    if(usbPort->isOpen()) usbPort->close();
    QString QsPortName = "NULL";
    if (nPortIndex > 0)
    {
        usbInfo = &availablePort.at(nPortIndex-1);
        QsPortName = usbInfo->portName();

        //funkcja setPort() dziedziczy wszystkie atrybuty portu typu BaudRate, DataBits, Parity itd.
        usbPort->setPort(availablePort.at(nPortIndex-1)); //todo: czy tu ju jest port otwarty/polaczony?
        if(!usbPort->open(QIODevice::ReadWrite)) //jezeli port nie jest otwarty
            this->addTextToUsbConsole("ERROR: Unable to open port", 'u');
    }
    else usbInfo = NULL; //wskaźnik czyszczony, by nie wyświetlało wcześniejszych informacji

    if (QsPortName != "NULL") //nie pokazuj próby podłączania do pustego portu
        this->addTextToUsbConsole("Connected to port: " + QsPortName + "\n", 'u');
}

void ArduinoUsb::sendDataToUsb(QString QsMsg) //wyslij wiadomość na serial port
{
    if(usbPort->isOpen())
    {
        emit this->addTextToUsbConsole(QsMsg + "\n", 'a');
        QsMsg += "$"; //wiadomości przez arduino odczytywane są póki nie natrafimy na znak '$'

        usbPort->write(QsMsg.toStdString().c_str());
        usbPort->waitForBytesWritten(-1); //TODO: czekaj w nieskonczonosć? co to jest
    }
    else emit this->addTextToUsbConsole("ERROR: USB port is closed\n", 'u');
}

void ArduinoUsb::readUsbData()
{
    QByteA_data = usbPort->readAll(); //zapisz w tablicy wszystko co przyszło z usb

    qDebug() << "QByteA_data = usbPort->readAll();" << QByteA_data;
    QString QsQByteA(QByteA_data);
    QsFullSerialMsg += QsQByteA;

    if(QsFullSerialMsg.at(0) == '@' && //jeżeli pierwszy znak wiadomosci to @...
            QsFullSerialMsg.at(QsFullSerialMsg.size()-1) == '$') //...a ostatni to $...
    {
        QsFullSerialMsg.remove('$'); //...to jest to cała wiadomość...
        QsFullSerialMsg.remove('@'); //... i pousuwaj te znaki.

        emit this->addTextToUsbConsole(QsFullSerialMsg + "\n", 'r');

        QByteA_data.clear();
        QsFullSerialMsg.clear();
    }
}

ArduinoUsb::~ArduinoUsb()
{
    if(usbPort->isOpen())
        usbPort->close();
}

#include "arduinousb.h"

ArduinoUsb::ArduinoUsb()
{
    usbPort = new QSerialPort(this);
    usbInfo = NULL; //wartośc wskażnika obecnie wybranego portu ustgawiamy na pustą wartość
    this->searchDevices(); //wyszukujemy obecnie podłączone urządzenia usb

    connect(usbPort, readyRead(), this, readUsbData());
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
void ArduinoUsb::portIndexChanged(int nIndex) //zmiana/wybór portu
{ //połączenie z portem jest ustawianie automatycznie jak tylko zostanie on wybrany
    if(usbPort->isOpen()) usbPort->close();
    QString QStrPortName = "NULL";
    if (nIndex > 0)
    {
        usbInfo = &availablePort.at(nIndex-1);
        QStrPortName = usbInfo->portName();

        //funkcja setPort() dziedziczy wszystkie atrybuty portu typu BaudRate, DataBits, Parity itd.
        usbPort->setPort(availablePort.at(nIndex-1)); //połącz z wybranym portem
        if(!usbPort->open(QIODevice::ReadWrite)) //jezeli port nie jest otwarty
            qDebug() << "ERROR: ArduinoUsb::portIndexChanged(): Unable to open port";
    }
    else usbInfo = NULL; //wskaźnik czyszczony, by nie wyświetlało wcześniejszych informacji

    if (QStrPortName != "NULL") //nie pokazuj próby podłączania do pustego portu
        emit this->addTextToLogPTE("Connected to port: " + QStrPortName + "\n", LOG_USB);
}

void ArduinoUsb::sendDataToUsb(QString QStrMsg) //wyslij wiadomość na serial port
{
    if(usbPort->isOpen())
    {
        emit this->addTextToLogPTE(QStrMsg + "\n", LOG_USB_SENT);
        QStrMsg += "$"; //wiadomości przez arduino odczytywane są póki nie natrafimy na znak '$'

        usbPort->write(QStrMsg.toStdString().c_str());
        usbPort->waitForBytesWritten(-1); //czekaj aż przyjdą wszystkie dane
    }
    else emit this->addTextToLogPTE("ERROR: USB port is closed\n", LOG_USB);
}

void ArduinoUsb::readUsbData()
{
    QByteA_data = usbPort->readAll(); //zapisz w tablicy wszystko co przyszło z usb

    qDebug() << "QByteA_data = usbPort->readAll():" << QByteA_data;
    QString QsQByteA(QByteA_data); //stwórz stringa z listy stringów z usb (musi być z dekalracją)
    QStrFullSerialMsg += QsQByteA; //sklejaj to w pozafunkcyjnym stringu, póki nie trafimy na '$'

    if(QStrFullSerialMsg.at(0) == '@' && //jeżeli pierwszy znak wiadomosci to @...
            QStrFullSerialMsg.at(QStrFullSerialMsg.size()-1) == '$') //...a ostatni to $...
    {
        QStrFullSerialMsg.remove('$'); //...to jest to cała wiadomość...
        QStrFullSerialMsg.remove('@'); //... i pousuwaj te znaki.

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

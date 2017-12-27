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
{ //połączenie z portem jest ustawianie automatycznie jak tylko zostanie on wybrany
    if(usbPort->isOpen()) usbPort->close();
    QString QsPortName = "NULL";
    if (nPortIndex > 0)
    {
        usbInfo = &availablePort.at(nPortIndex-1);
        QsPortName = usbInfo->portName();

        //funkcja setPort() dziedziczy wszystkie atrybuty portu typu BaudRate, DataBits, Parity itd.
        usbPort->setPort(availablePort.at(nPortIndex-1)); //połącz z wybranym portem
        if(!usbPort->open(QIODevice::ReadWrite)) //jezeli port nie jest otwarty
            this->addTextToConsole("ERROR: Unable to open port\n", LOG_USB);
    }
    else usbInfo = NULL; //wskaźnik czyszczony, by nie wyświetlało wcześniejszych informacji

    if (QsPortName != "NULL") //nie pokazuj próby podłączania do pustego portu
    {
        this->addTextToConsole("Connected to port: " + QsPortName + "\n", LOG_USB);
        qDebug() << "Connected to port: " << QsPortName;
    }
}

void ArduinoUsb::sendDataToUsb(QString QsMsg) //wyslij wiadomość na serial port
{
    if(usbPort->isOpen())
    {
        emit this->addTextToConsole(QsMsg + "\n", LOG_USB_SENT);
        QsMsg += "$"; //wiadomości przez arduino odczytywane są póki nie natrafimy na znak '$'

        usbPort->write(QsMsg.toStdString().c_str());
        usbPort->waitForBytesWritten(-1); //czekaj aż przyjdą wszystkie dane
    }
    else emit this->addTextToConsole("ERROR: USB port is closed\n", LOG_USB);
}

void ArduinoUsb::readUsbData()
{
    QByteA_data = usbPort->readAll(); //zapisz w tablicy wszystko co przyszło z usb

    qDebug() << "QByteA_data = usbPort->readAll():" << QByteA_data;
    QString QsQByteA(QByteA_data); //stwórz stringa z listy stringów z usb (musi być z dekalracją)
    QsFullSerialMsg += QsQByteA; //sklejaj to w pozafunkcyjnym stringu, póki nie trafimy na '$'

    if(QsFullSerialMsg.at(0) == '@' && //jeżeli pierwszy znak wiadomosci to @...
            QsFullSerialMsg.at(QsFullSerialMsg.size()-1) == '$') //...a ostatni to $...
    {
        QsFullSerialMsg.remove('$'); //...to jest to cała wiadomość...
        QsFullSerialMsg.remove('@'); //... i pousuwaj te znaki.

        emit this->addTextToConsole(QsFullSerialMsg + "\n", LOG_USB_RECEIVED);
        this->manageMsgFromUsb(QsFullSerialMsg);

        QByteA_data.clear();
        QsFullSerialMsg.clear();
    }
}

void ArduinoUsb::manageMsgFromUsb(QString QStrMsg)
{
    if (QStrMsg == "reset")
        emit this->restoreGameToInitialState();
    else if (QStrMsg == "start")
        emit this->TcpQueueMsg(ARDUINO, "think 5000"); //think->save move -> undo
    else if (QStrMsg.left(4) == "move")
        emit this->AIEnemySend(QStrMsg);
    else if (QStrMsg.left(9) == "promoteTo")
        emit this->AIEnemySend(QStrMsg.left(10));
    else
    {
        emit this->addTextToConsole("ERROR: unknown command from usb: " +
                                    QStrMsg + "\n", LOG_USB_RECEIVED);
        qDebug() << "ERROR: ArduinoUsb::manageMsgFromUsb: unknown"
                    " command from usb:" << QStrMsg;
    }
}

ArduinoUsb::~ArduinoUsb()
{
    if(usbPort->isOpen())
        usbPort->close();
}

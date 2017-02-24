#include "arduinousb.h"

ArduinoUsb::ArduinoUsb()
{
    usbPort = new QSerialPort(this);
    usbInfo = NULL; //wartośc wskażnika obecnie wybranego portu ustgawiamy na pustą wartość
    this->searchDevices(); //wyszukujemy obecnie podłączone urządzenia usb

    /*//łączymy sygnał wciśnięcia przycisku w menu odpowiadającego za żądanie
    //dodatkowego zaktualizowania listy portów z odpowiednim slotem
    connect(ui->action_refreshPorts, SIGNAL(triggered()), this, SLOT(refresh()));*/
    //poszło do mainwindow

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
void ArduinoUsb::portIndexChanged(int index) //zmiana/wybór portu
{ //bodajze jest ustawianie połączenie z portem automatycznie jak tylko zostanie on wybrany
    if(usbPort->isOpen()) usbPort->close();
    QString txt = "NULL";
    if (index > 0)
    {
        usbInfo = &availablePort.at(index-1);
        txt = usbInfo->portName();

        //funkcja setPort() dziedziczy wszystkie atrybuty portu typu BaudRate, DataBits, Parity itd.
        usbPort->setPort(availablePort.at(index-1)); //todo: czy tu ju jest port otwarty/polaczony?
        if(!usbPort->open(QIODevice::ReadWrite)) //jezeli port nie jest otwarty
            //QMessageBox::warning(this,"Device error04","Unable to open port."); //wyrzuć error
            this->addTextToUsbConsole("ERROR: Unable to open port", 'u');
        /*TODO: else //a jeżeli jest port otwarty, to łap sygnay z portu i wrzucaj do slotu
            connect(port,SIGNAL(readyRead()),this,SLOT(readUsbData()));
        */
    }
    else
        //wskaźnik czyszczony, by nie wskazywał wcześniejszych informacji ze wskaźnika z pamięci
        usbInfo = NULL;

    this->addTextToUsbConsole("Połączono z portem: " + txt, 'u');
    //ui->statusBar->showMessage("Połączono z portem: " + txt, 5000);
}

/*void ArduinoUsb::on_commandLine_returnPressed() //wciśnięcie przycisku entera
{
    // jeżeli nie wybrano żadnego urządzenia nie wysyłamy
    if(usbInfo == NULL)
    {
        this->addTextToUsbConsole("Not port selected", 'u');
       // ui->statusBar->showMessage("Not port selected", 3000);
        return;
    }

    this->sendDataToUsb(ui->commandLine->text(), true); //zczytaj wiadomośc z pola textowe i wyślij na usb
    ui->commandLine->clear(); // wyczyść pole textowe
}*/

void ArduinoUsb::sendDataToUsb(QString QsMsg/*, bool sender*/) //wyslij wiadomość na serial port
{
    if(usbPort->isOpen() && QsMsg != "error05") //TODO: opis erroru
    {
        emit this->addTextToUsbConsole(QsMsg, 'u');

        usbPort->write(QsMsg.toStdString().c_str()); //TODO: +'$' nie działa z tym
        usbPort->waitForBytesWritten(-1); //TODO: czekaj w nieskonczonosć??? co to jest
    }
    else emit this->addTextToUsbConsole("error06", 'u'); //TODO: opis erroru
}

void ArduinoUsb::readUsbData()
{
    QByteArray QByteA_data; // tablica niezorganizowanych danych przypływających z usb
    QByteA_data = usbPort->readAll(); //zapisz w tablicy wszystko co przyszło z usb
    while(usbPort->waitForReadyRead(100)) // ?? może dać mniej czasu?? będzie to wtedy działało szybciej??
        QByteA_data += usbPort->readAll(); //składamy tutaj wszystkie dane które przyszły w 1 zmienną

    QString QsFullSerialMsg(QByteA_data); //tablicę znaków zamienamy na Qstring

    //TODO: ogarnąć start i końce wiadomości
    if(QsFullSerialMsg.at(0) == '@' && //jeżeli pierwszy znak wiadomosci to @...
            QsFullSerialMsg.at(QsFullSerialMsg.size()-1) == '$') //...a ostatni to $...
    {
        QsFullSerialMsg.remove('$'); //...to jest to cała wiadomość...
        QsFullSerialMsg.remove('@'); //... i pousuwaj te znaki.

        emit this->addTextToUsbConsole(QsFullSerialMsg + "\n", 'u');

        //TODO: tu zrób coś z wiadomością

        QByteA_data.clear();
    }
    else emit this->addTextToUsbConsole("error07: " + QsFullSerialMsg, 'u'); //TODO: opis erroru
}

ArduinoUsb::~ArduinoUsb()
{
    if(usbPort->isOpen())
        usbPort->close();
}

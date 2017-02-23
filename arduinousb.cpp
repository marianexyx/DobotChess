#include "arduinousb.h"

ArduinoUsb::ArduinoUsb()
{
    usb_port = new QSerialPort(this);
    info = NULL; //wartośc wskażnika obecnie wybranego portu ustgawiamy na pustą wartość
    searchDevices(); //wyszukujemy obecnie podłączone urządzenia usb

    //łączymy sygnał wciśnięcia przycisku w menu odpowiadającego za żądanie
    //dodatkowego zaktualizowania listy portów z odpowiednim slotem
    connect(ui->action_refreshPorts, SIGNAL(triggered()), this, SLOT(refresh()));

    connect(usb_port, SIGNAL(readyRead()), this, SLOT(readUsbData()));
}


//aktualizowanie listy z urządzeniami
//tutaj wyszukujemy obecnie podłączone urządzenia usb i „wrzucamy” je do wcześniej przygotowanej listy
void ArduinoUsb::searchDevices()
{
    // dodanie ich do listy
    available_port = QSerialPortInfo::availablePorts();

    int porty = available_port.size();
    QString message = QString::number(porty)
            + (porty == 1 ? " port is ready to use" : " ports are ready to use");

    // wyświetlamy wiadomość z informacją ile znaleźliwśmy urządzeń gotowych do pracy
    ui->statusBar->showMessage(message,3000); //w stopce okna

    // aktualizujemy
    info = NULL;
    ui->port->clear();
    ui->port->addItem("NULL");
    for(int i=0;i<porty;i++)
    {
        ui->port->addItem(available_port.at(i).portName()); //wypełnianie combo boxa portami
    }
}

//Slot jest aktywowany po zmianie wartości przez użytkownika w combo box’ie. Ustawia on wskaźnik
//na nowy obiekt (lub nic) i wyświetla odpowiednią informację w pasku statusu
void ArduinoUsb::on_port_currentIndexChanged(int index) //zmiana/wybór portu
{ //bodajze jest ustawianie połączenie z portem automatycznie jak tylko zostanie on wybrany
    if(usb_port->isOpen()) usb_port->close();
    QString txt = "NULL";
    if (index > 0)
    {
        info = &available_port.at(index-1);
        txt = info->portName();

        //funkcja setPort() dziedziczy wszystkie atrybuty portu typu BaudRate, DataBits, Parity itd.
        usb_port->setPort(available_port.at(index-1)); //todo: czy tu ju jest port otwarty/polaczony?
        if(!usb_port->open(QIODevice::ReadWrite)) //jezeli port nie jest otwarty
            QMessageBox::warning(this,"Device error04","Unable to open port."); //wyrzuć error
        /*else //a jeżeli jest port otwarty, to łap sygnay z portu i wrzucaj do slotu
            connect(port,SIGNAL(readyRead()),this,SLOT(readUsbData()));
        */
    }
    else
        //wskaźnik czyszczony, by nie wskazywał wcześniejszych informacji ze wskaźnika z pamięci
        info = NULL;

    ui->statusBar->showMessage("Połączono z portem: " + txt,5000);
}

void ArduinoUsb::refresh()
{
    searchDevices();
}

void ArduinoUsb::on_commandLine_returnPressed() //wciśnięcie przycisku entera
{
    // jeżeli nie wybrano żadnego urządzenia nie wysyłamy
    if(info == NULL) {
        ui->statusBar->showMessage("Not port selected",3000);
        return;
    }

    sendDataToUsb(ui->commandLine->text(),true); //zczytaj wiadomośc z pola textowe i wyślij na usb
    ui->commandLine->clear(); // wyczyść pole textowe
}

void ArduinoUsb::sendDataToUsb(QString QS_msg, bool sender) //wyslij wiadomość na serial port
{
    if(usb_port->isOpen() && QS_msg != "error05")
    {
        addTextToUsbConsole(QS_msg, sender);

        usb_port->write(QS_msg.toStdString().c_str()); // +'$'   -nie działa z tym
        usb_port->waitForBytesWritten(-1); //??? czekaj w nieskonczonosć??? co to jest
    }
    else addTextToUsbConsole("error06", true);
}

void ArduinoUsb::readUsbData()
{
    QByteArray QByteA_data; // tablica niezorganizowanych danych przypływających z usb
    QByteA_data = usb_port->readAll(); //zapisz w tablicy wszystko co przyszło z usb
    while(usb_port->waitForReadyRead(100)) // ?? może dać mniej czasu?? będzie to wtedy działało szybciej??
        QByteA_data += usb_port->readAll(); //składamy tutaj wszystkie dane które przyszły w 1 zmienną

    QString QS_fullSerialMsg(QByteA_data); //tablicę znaków zamienamy na Qstring

    if(QS_fullSerialMsg.at(0) == '@' && //jeżeli pierwszy znak wiadomosci to @...
            QS_fullSerialMsg.at(QS_fullSerialMsg.size()-1) == '$') //...a ostatni to $...
    {
        QS_fullSerialMsg.remove('$'); //...to jest to cała wiadomość...
        QS_fullSerialMsg.remove('@'); //... i pousuwaj te znaki.

        addTextToUsbConsole(QS_fullSerialMsg + "\n", false);

        if (QS_fullSerialMsg.left(2) == "n_") normalPieceMoving(QS_fullSerialMsg);
        else if (QS_fullSerialMsg.left(2) == "r_") removePieceSequence(QS_fullSerialMsg);
        else if (QS_fullSerialMsg.left(2) == "c_") castlingMovingSequence(QS_fullSerialMsg);

        QByteA_data.clear();
    }
    else addTextToUsbConsole("error07: " + QS_fullSerialMsg, false);
}

ArduinoUsb::~ArduinoUsb()
{
    if(usb_port->isOpen())
        usb_port->close();
}

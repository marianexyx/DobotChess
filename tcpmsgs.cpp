#include "tcpmsgs.h"
#include <QDataStream>


//TODO: jakaś normalizacja i odpowiednie miejsce na constanty
const int WEBSITE = 1;
const int ARDUINO = 2;

TCPMsgs::TCPMsgs()
{
    m_bWaitingForReadyRead = false;
    m_ullID = 0;
}

void TCPMsgs::queueMsgs(int nSender, QString msg)
{
    qDebug() << "TCPMsgs::queueMsgs received msg: " << msg << ", sender = " << nSender;

    TcpMsgMetadata QStrReceivedData;
    QStrReceivedData.ullTcpID = ++m_ullID;
    QStrReceivedData.nSender = nSender;
    QStrReceivedData.QStrMsgForTcp = msg;
    TCPMsgsList << QStrReceivedData; //wrzuć do kontenera wiadomość
    if (!TCPMsgsList.isEmpty()) //jeżeli kontener nie jest pusty
    {
        if (!m_bWaitingForReadyRead) //zaczekaj z kolejkowaniem kolejnego zapytania do TCP jeżeli...
            //...czekamy aktualnie na przetworzenie jakiejś odpowiedzi z TCP
            this->doTcpConnect(); //to wykonaj najstarszą wiadomość z kontenera.
    }
}

void TCPMsgs::TcpQueueMsg(int nSender, QString msg)
{
    this->queueMsgs(nSender, msg);
}

//rozmowa z tcp. każde 1 polecenie tworzy 1 instancję rozmowy z tcp.
void TCPMsgs::doTcpConnect()
{
    m_bWaitingForReadyRead = true;

    socket = new QTcpSocket(this);

    //każde nowe zapytanie jest nowym połączeniem
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64))); //to mi raczej zbędne
    //connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, &QIODevice::readyRead, this, &TCPMsgs::readyRead);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, displayError);

    qDebug() << "TCPMsgs: connecting...";

    socket->abort(); //pozwoli to zakończyć stare połączenie jeżeli jeszcze nie zostało zerwane...
    //...by można było stworzyć nowe

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //docelowo emituje sygnał connected (tcp)

    if(!socket->waitForConnected(5000)) //czkemy na połączenie 5 sekund
    {
        emit addTextToConsole("Error12:" + socket->errorString() + "\n", 't');
    }
}

void TCPMsgs::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break; //server protocol ends with the server closing the connection
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "ERROR: TCPMsgs: The host was not found. Please check the"
                    "host name and port settings." ;
        emit addTextToConsole("ERROR: TCPMsgs: The host was not found. Please check the"
                              "host name and port settings.\n", 't');
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ERROR: TCPMsgs: The connection was refused by the peer. Make sure the server is "
                    "running, and check that the host name and port settings are correct.";
        emit addTextToConsole("ERROR: TCPMsgs: The connection was refused by the peer. Make sure the server is "
                              "running, and check that the host name and port settings are correct.", 't');
        break;
    default:
        qDebug() << "ERROR: TCPMsgs: The following error occurred:" << socket->errorString();
        emit addTextToConsole("ERROR: TCPMsgs: The following error occurred: " + socket->errorString(), 't');
    }
}

void TCPMsgs::connected() //udało się nawiązać połączenie z tcp
{
    TcpMsgMetadata QStrData;
    if (!TCPMsgsList.isEmpty()) QStrData = TCPMsgsList.last();
    else
    {
        qDebug() << "ERROR: TCPMsgs::connected(): TCPMsgsList is empty";
        return;
    }

    qDebug() << "TCPMsgs: connected...";
    qDebug() << "TCPMsgs: parsing msg to chenard:" << QStrData.QStrMsgForTcp;

    QByteArray QabMsgArrayed;
    QabMsgArrayed.append(QStrData.QStrMsgForTcp + "\n"); //przetworzenie parametru dla funkcji write()
    // send msg to tcp from sender. chenard rozumie koniec wiadomości poprzez "\n"
    socket->write(QabMsgArrayed); //write wysyła wiadomość (w bajtach) na server przez tcp

    emit addTextToConsole("wrote to TCP: " + QabMsgArrayed, 't');
}

void TCPMsgs::disconnected()
{
    qDebug() << "disconnected...";
}

void TCPMsgs::bytesWritten(qint64 bytes) //mówi nam ile bajtów wysłaliśmy do tcp
{
    qDebug() << "TCPMsgs: " << QString::number(bytes) << " bytes written...";
}

void TCPMsgs::readyRead() //funckja odbierająca odpowiedź z tcp z wcześniej wysłanej wiadmoności
{
    qDebug() << "TCPMsgs: reading...";

    QString QStrMsgFromTcp;
    do
    {
        if (!QStrMsgFromTcp.isEmpty())
            qDebug() << "WARNING: TCPMsgs::readyRead- needed to read data from socket 1 more time";
        QStrMsgFromTcp += socket->readAll();
        if (socket->bytesAvailable() > 0)
            qDebug() << "WARNING: TCPMsgs::readyRead- socket->bytesAvailable() > 0 after 1st read";
    }
    while (socket->bytesAvailable() > 0);

    //TODO: dostaję czasem rozklejone końcówki w bodajże kolejnej wiadomości: "" lub "/n". Póki co...
    //zostało to naprawione półśrodkiems
    if (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n") //jeżeli nie jest to końcówka/syf
    {
        if (QStrMsgFromTcp.right(1) != "\n") QStrMsgFromTcp += "\n";
        emit addTextToConsole("tcp answer: " + QStrMsgFromTcp, 't');

        TcpMsgMetadata QStrData;
        if (!TCPMsgsList.isEmpty())
            QStrData = TCPMsgsList.takeLast();
        else
        {
            qDebug() << "ERROR: TCPMsgs::readyRead(): TCPMsgsList is empty";
            return;
        }

        if (QStrData.nSender == WEBSITE) //inna klasa powinna o tym decydowac?
        {
            qDebug() << "ID =" << QStrData.ullTcpID << ", sender =" << QStrData.nSender <<
                        ", msgForTcp =" << QStrData.QStrMsgForTcp << ", msgFromTcp =" << QStrMsgFromTcp;
            //core niech odbierze tą wiadomość i zdecyduje do dalej
            emit this->msgFromTcpToWeb(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
            //m_bWaitingForReadyRead = false;
        }
        else if (QStrData.nSender == ARDUINO)
        {
            qDebug() << "ID =" << QStrData.ullTcpID << ", sender =" << QStrData.nSender <<
                        ", msgForTcp =" << QStrData.QStrMsgForTcp << ", msgFromTcp =" << QStrMsgFromTcp;
            emit this->msgFromTcpToArd(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
            //m_bWaitingForReadyRead = false;
        }
        else qDebug() << "ERROR: TCPMsgs::readyRead(): unknown QStrData.nSender value =" <<
                         QStrData.nSender;
    }
    else if (QStrMsgFromTcp.isEmpty())
    {
        qDebug() << "ERROR: TCPMsgs::readyRead() received empty msg.";
        return;
    }
    else if (QStrMsgFromTcp == "\n")
        qDebug() << "ERROR: TCPMsgs::readyRead() received '\\n' msg.";
    else qDebug() << "ERROR: TCPMsgs::readyRead() received:" << QStrMsgFromTcp;

    m_bWaitingForReadyRead = false;

    if (!TCPMsgsList.isEmpty()) //jeżeli pozostały jeszcze jakieś zapytania do tcp do przetworzenia
    {
        this->doTcpConnect(); //to je wykonaj
    }
}

#include "tcpmsgs.h"
#include <QDataStream>

TCPMsgs::TCPMsgs()
{
    _bWaitingForReadyRead = false;
    _n64CmdID = 0;
}

void TCPMsgs::queueCmd(COMMUNICATION_TYPE Sender, QString QStrCmd)
{
    qDebug() << "TCPMsgs::queueCmd(): cmd =" << QStrCmd << ", sender ="
             << communicationTypeAsQStr(Sender);

    TcpMsgMetadata QStrReceivedData;
    QStrReceivedData.n64TcpID = ++_n64CmdID;
    QStrReceivedData.nSender = Sender;
    QStrReceivedData.QStrMsgForTcp = QStrCmd;
    TCPMsgsList << QStrReceivedData;
    if (!TCPMsgsList.isEmpty() && !_bWaitingForReadyRead)
    {
        //zaczekaj z wykonywaniem kolejnego zapytania do TCP z kontenera...
        //...jeżeli czekamy aktualnie na przetworzenie jakiejś odpowiedzi z TCP
        this->doTcpConnect(); //to wykonaj najstarszą wiadomość z kontenera.
    }
}

//rozmowa z tcp. każde 1 polecenie tworzy 1 instancję rozmowy z tcp
void TCPMsgs::doTcpConnect()
{
    _bWaitingForReadyRead = true;

    socket = new QTcpSocket(this);

    //każde nowe zapytanie jest nowym połączeniem
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64))); //to mi raczej zbędne
    connect(socket, &QIODevice::readyRead, this, &TCPMsgs::readyRead);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, displayError);

    qDebug() << "TCPMsgs::doTcpConnect(): connecting...";

    socket->abort(); //pozwoli to zakończyć stare połączenie jeżeli jeszcze nie zostało zerwane...
    //...by można było stworzyć nowe

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //docelowo emituje sygnał connected (tcp)

    if(!socket->waitForConnected(5000))
    {
        qDebug() << "ERROR: TCPMsgs::doTcpConnect():" << socket->errorString();
    }
}

void TCPMsgs::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break; //server protocol ends with the server closing the connection
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "ERROR: TCPMsgs::doTcpConnect(): The host was not found. Please "
                    "check the host name and port settings." ;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "ERROR: TCPMsgs::doTcpConnect(): The connection was refused by the "
                    "peer. Make sure the server is running, and check that the host name "
                    "and port settings are correct.";
        break;
    default:
        qDebug() << "ERROR: TCPMsgs::doTcpConnect(): The following error occurred:"
                 << socket->errorString();
    }
}

void TCPMsgs::connected()
{
    TcpMsgMetadata QStrData;
    if (!TCPMsgsList.isEmpty())
    {
        QStrData = TCPMsgsList.last();

        qDebug() << "TCPMsgs::connected(): Connected. Parsing msg to chenard:"
                 << QStrData.QStrMsgForTcp;

        QByteArray QabMsgArrayed;
        QabMsgArrayed.append(QStrData.QStrMsgForTcp + "\n"); //przetworzenie parametru dla funkcji write()
        // send msg to tcp from sender. chenard rozumie koniec wiadomości poprzez "\n"
        //future: mozliwe ze tu wyzej tworzone sa wiadomosci konczace sie na "\n\n"?
        socket->write(QabMsgArrayed); //write wysyła wiadomość (w bajtach) na server przez tcp

        emit this->addTextToLogPTE("wrote to TCP: " + QabMsgArrayed, LOG_TCP);
    }
    else
    {
        qDebug() << "ERROR: TCPMsgs::connected(): TCPMsgsList is empty";
        return;
    }
}

void TCPMsgs::disconnected()
{
    qDebug() << "TCPMsgs::disconnected()";
}

void TCPMsgs::bytesWritten(qint64 bytes) //mówi nam ile bajtów wysłaliśmy do tcp
{
    qDebug() << "TCPMsgs::bytesWritten(): " << QString::number(bytes) << " bytes written...";
}

void TCPMsgs::readyRead() //funckja odbierająca odpowiedź z tcp z wcześniej wysłanej wiadmoności
{
    qDebug() << "TCPMsgs::readyRead(): reading...";

    QString QStrMsgFromTcp;
    do
    {
        if (!QStrMsgFromTcp.isEmpty())
            qDebug() << "WARNING: TCPMsgs::readyRead- needed to read data from"
                        " socket 1 more time";

        QStrMsgFromTcp += socket->readAll();

        if (socket->bytesAvailable() > 0)
            qDebug() << "WARNING: TCPMsgs::readyRead- socket->bytesAvailable() > 0"
                        " after 1st read";
    }
    while (socket->bytesAvailable() > 0);

    //future: dostaję czasem rozklejone końcówki w bodajże kolejnej wiadomości: '' lub
    //'/n'. Póki co zostało to naprawione półśrodkiem
    if (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n") //jeżeli nie jest to końcówka/syf
    {
        if (QStrMsgFromTcp.right(1) != "\n")
            QStrMsgFromTcp += "\n";

        emit this->addTextToLogPTE("tcp answer: " + QStrMsgFromTcp, LOG_TCP);

        TcpMsgMetadata QStrData;
        if (!TCPMsgsList.isEmpty())
            QStrData = TCPMsgsList.takeLast();
        else
        {
            qDebug() << "ERROR: TCPMsgs::readyRead(): TCPMsgsList is empty";
            return;
        }

        qDebug() << "TCPMsgs::readyRead(): ID =" << QStrData.n64TcpID << ", sender ="
                 << QStrData.nSender << ", msgForTcp =" << QStrData.QStrMsgForTcp
                 << ", msgFromTcp =" << QStrMsgFromTcp;

        emit this->msgFromTcpToChess(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
    }
    else if (QStrMsgFromTcp.isEmpty())
    {
        qDebug() << "WARNING: TCPMsgs::readyRead(): received empty msg.";
        return;
    }
    else if (QStrMsgFromTcp == "\n")
        qDebug() << "WARNING: TCPMsgs::readyRead(): received '\\n' msg.";
    else
        qDebug() << "WARNING: TCPMsgs::readyRead(): received:" << QStrMsgFromTcp;

    _bWaitingForReadyRead = false;

    //jeżeli pozostały jeszcze jakieś zapytania do tcp do przetworzenia
    if (!TCPMsgsList.isEmpty())
    {
        this->doTcpConnect(); //to je wykonaj
    }
}

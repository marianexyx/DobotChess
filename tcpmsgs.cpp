#include "tcpmsgs.h"
#include <QDataStream>

TCPMsgs::TCPMsgs()
{
    _bWaitingForReadyRead = false;
    _n64CmdID = 0;
}

//todo: make queue as seperate thread
void TCPMsgs::queueCmd(QString QStrCmd)
{
    qDebug() << "TCPMsgs::queueCmd(): cmd =" << QStrCmd;

    TcpMsgMetadata QStrReceivedData;
    QStrReceivedData.n64TcpID = ++_n64CmdID;
    QStrReceivedData.QStrMsgForTcp = QStrCmd;
    TCPMsgsList << QStrReceivedData;
    if (!TCPMsgsList.isEmpty() && !_bWaitingForReadyRead)
    {
        //wait with next TCP command execution from container, till earlier one isn't ended
        this->doTcpConnect(); //execute oldest command in container
    }
    //todo: else kill this thread
}

//conversation with tcp. every 1 command create 1 tcp instation
void TCPMsgs::doTcpConnect()
{
    _bWaitingForReadyRead = true;

    socket = new QTcpSocket(this);

    //every new command is new connection
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    //bytesWritten is quite useless for me, but looks like it's necessary for tcp to work
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, &QIODevice::readyRead, this, &TCPMsgs::readyRead);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, displayError);

    qDebug() << "TCPMsgs::doTcpConnect(): connecting...";

    socket->abort();
    //this allow to end old connection, if it wasn't closed yet, making a place for new one
    //future: make a warning here

    // this is not blocking call
    socket->connectToHost("localhost", 22222); //will emit signal "connected"

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
        //prepare parameter for write() function
        QabMsgArrayed.append(QStrData.QStrMsgForTcp + "\n");
        //send msg to tcp from sender. chenard understand end of msg, when encounter "\n" msg
        //future: propably above warning "\n\n" messages are created?
        socket->write(QabMsgArrayed);

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

void TCPMsgs::bytesWritten(qint64 bytes)
{
    qDebug() << "TCPMsgs::bytesWritten(): " << QString::number(bytes) << " bytes written...";
}

void TCPMsgs::readyRead()
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

    //future: sometimes im encountering bad end of messages, that will be catched as new answers.
    //... they manifest through additional whitespaces at the end of a msg, like """, or "/n".
    //walkaround has been used
    if (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n")
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

        qDebug() << "TCPMsgs::readyRead(): ID =" << QStrData.n64TcpID
                 << ", msgForTcp =" << QStrData.QStrMsgForTcp
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

    if (!TCPMsgsList.isEmpty())
    {
        this->doTcpConnect();
    }
}

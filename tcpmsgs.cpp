#include "tcpmsgs.h"
#include <QDataStream>

TCPMsgs::TCPMsgs()
{
    m_bWaitingForReadyRead = false;
    m_n64CmdID = 0;
}

void TCPMsgs::queueCmd(QString QStrCmd)
{
    qInfo() << "cmd =" << QStrCmd;

    TcpMsgMetaData QStrReceivedData;
    QStrReceivedData.un64TcpID = ++m_n64CmdID;
    QStrReceivedData.QStrMsgForTcp = QStrCmd;
    m_TCPMsgsList << QStrReceivedData;
    if (!m_TCPMsgsList.isEmpty() && !m_bWaitingForReadyRead)
    {
        //wait with next TCP command execution from container, till earlier one isn't ended
        this->doTcpConnect(); //execute oldest command in container
    }
}

void TCPMsgs::doTcpConnect() //every 1 command create 1 tcp instation
{
    m_bWaitingForReadyRead = true;

    m_socket = new QTcpSocket(this);

    //every new command is new connection
    connect(m_socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(m_socket, &QIODevice::readyRead, this, &TCPMsgs::readyRead);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(m_socket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            this, displayError);

    qInfo() << "connecting...";

    //abort allow to end old connection, if it wasn't closed yet, making a place for new one
    m_socket->abort(); //todo: make a warning here, if there was an old connection

    m_socket->connectToHost("localhost", 22222); //will emit signal "connected"
    //future: add additional connectToHost reaction, when it doesn't respond for...
    //...too long (use timer, not "waitForConnect" function). edit: is it necessary?
}

void TCPMsgs::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break; //server protocol ends with the server closing the connection
    case QAbstractSocket::HostNotFoundError:
        qCritical() << "The host was not found. Check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qCritical() << "The connection was refused by the peer. Make sure the server is running,"
                       " and check that the host name and port settings are correct.";
        break;
    default: qCritical() << m_socket->errorString();
    }
}

void TCPMsgs::connected()
{
    TcpMsgMetaData QStrData;
    if (!m_TCPMsgsList.isEmpty())
    {
        QStrData = m_TCPMsgsList.last();

        qInfo() << "Connected. Parsing msg to chenard:" << QStrData.QStrMsgForTcp;

        QByteArray QabMsgArrayed;
        //prepare parameter for write() function
        QabMsgArrayed.append(QStrData.QStrMsgForTcp + "\n");
        //send msg to tcp from sender. chenard understand end of msg, when encounter "\n" msg
        //future: propably above warning "\n\n" messages are created?
        m_socket->write(QabMsgArrayed);

        emit this->addTextToLogPTE("wrote to TCP: " + QabMsgArrayed, LOG_TCP);
    }
    else
    {
        qCritical() << "m_TCPMsgsList is empty";
        return;
    }
}

void TCPMsgs::readyRead()
{
    qInfo() << "reading...";

    QString QStrMsgFromTcp;
    do
    {
        if (!QStrMsgFromTcp.isEmpty())
            qWarning() << "needed to read data from m_socket 1 more time";

        QStrMsgFromTcp += m_socket->readAll();

        if (m_socket->bytesAvailable() > 0)
            qWarning() << "m_socket->bytesAvailable() > 0 after 1st read";
    }
    while (m_socket->bytesAvailable() > 0);

    //future: sometimes im encountering bad end of messages, that will be catched as new answers.
    //... they manifest through additional whitespaces at the end of a msg, like """, or "/n".
    //walkaround has been used
    if (!QStrMsgFromTcp.isEmpty() && QStrMsgFromTcp != "\n")
    {
        if (QStrMsgFromTcp.right(1) != "\n")
            QStrMsgFromTcp += "\n";

        emit this->addTextToLogPTE("tcp answer: " + QStrMsgFromTcp, LOG_TCP);

        TcpMsgMetaData QStrData;
        if (!m_TCPMsgsList.isEmpty())
            QStrData = m_TCPMsgsList.takeLast();
        else
        {
            qCritical() << "m_TCPMsgsList is empty";
            return;
        }

        qInfo() << "ID =" << QString::number(QStrData.un64TcpID) << ", msgForTcp ="
                << QStrData.QStrMsgForTcp << ", msgFromTcp =" << QStrMsgFromTcp;

        emit this->msgFromTcpToChess(QStrData.QStrMsgForTcp, QStrMsgFromTcp);
    }
    else if (QStrMsgFromTcp.isEmpty())
    {
        qWarning() << "received empty msg.";
        return;
    }
    else if (QStrMsgFromTcp == "\n")
        qWarning() << "received '\\n' msg.";
    else
        qWarning() << "received:" << QStrMsgFromTcp;

    m_bWaitingForReadyRead = false;

    if (!m_TCPMsgsList.isEmpty())
        this->doTcpConnect();
}

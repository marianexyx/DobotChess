#include "tcpmsgs.h"

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

    //wait with next TCP command execution from container, till earlier one isn't ended
    if (!m_TCPMsgsList.isEmpty() && !m_bWaitingForReadyRead)
        this->doTcpConnect(); //execute oldest command in container
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
    if (m_socket->state() == QAbstractSocket::ConnectingState
            || m_socket->state() == QAbstractSocket::ConnectingState)
        qCritical() << "aborting established connection";

    m_socket->abort();

    //connectToHost will emit signal "connected", or error signal type, when an error occurs
    m_socket->connectToHost("localhost", 22222);
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

        //future: "\n\n" messages are created here?
        QabMsgArrayed.append(QStrData.QStrMsgForTcp + "\n"); //prepare param for write() func
        //send msg to tcp from sender. chenard understand end of msg, when encounter "\n" msg

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
    //future: looks like i haven't seen this warning types for a longer while
    else if (QStrMsgFromTcp.isEmpty())
    {
        qWarning() << "received empty msg.";
        return;
    }
    else if (QStrMsgFromTcp == "\n")  qWarning() << "received '\\n' msg.";
    else qWarning() << "received:" << QStrMsgFromTcp;

    m_bWaitingForReadyRead = false;

    if (!m_TCPMsgsList.isEmpty())
        this->doTcpConnect();
}

QString TCPMsgs::dumpAllData()
{
    QString QStrData;

    QStrData = "[tcpmsgs.h]";
    QStrData += "m_bWaitingForReadyRead: " + QString::number(m_bWaitingForReadyRead) + "\n";
    QStrData += "m_n64CmdID: " + QString::number(m_n64CmdID) + "\n";
    foreach (TcpMsgMetaData tcpData, m_TCPMsgsList)
    {
        QStrData += "[m_TCPMsgsList] un64TcpID: " + QString::number(tcpData.un64TcpID)
                + ", QStrMsgForTcp: " + tcpData.QStrMsgForTcp + "\n";
    }

    return QStrData;
}

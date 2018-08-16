#ifndef TCPMSGS_H
#define TCPMSGS_H

#pragma once
#include <QTcpSocket>
#include <QString>
#include "vars/log.h"
#include "vars/const_flags.h"

struct TcpMsgMetaData
{
    uint64_t un64TcpID;
    QString QStrMsgForTcp;
};

class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket* m_socket;
    QList<TcpMsgMetaData> m_TCPMsgsList;

    bool m_bWaitingForReadyRead; //this flag blocks sending next msg...
    //...to tcp from queue, if previous msg hasn't been processed
    int64_t m_n64CmdID;

    void doTcpConnect();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();
    void readyRead();

public:
    TCPMsgs();

public slots:
    void queueCmd(QString QStrCmd);

signals:
    void addTextToLogPTE(QString, LOG);
    void msgFromTcpToChess(QString, QString);
};

#endif // TCPMSGS_H

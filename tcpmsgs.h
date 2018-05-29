#ifndef TCPMSGS_H
#define TCPMSGS_H

#pragma once
#include <QTcpSocket>
#include <QString>
#include "vars/log.h"
#include "vars/basic_vars.h"

struct TcpMsgMetadata
{
    int64_t n64TcpID;
    int nSender;
    QString QStrMsgForTcp;
};

class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket* socket;
    QList<TcpMsgMetadata> TCPMsgsList;

    bool _bWaitingForReadyRead; //flaga dzięki której zablokujemy możliwość wykonywania od razu...
    //...zapytania z kolejki zapytań do TCP jeżeli aktualnie jest przetwarzane jakieś zapytanie...
    //...na którego odpowiedź(przetworzenie) z TCP czekamy
    int64_t _n64CmdID;
    void doTcpConnect(); //todo: program czeka aż połączenie się wykona- zwątkować to

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

public slots:
    void queueCmd(COMMUNICATION_TYPE Sender, QString QStrCmd);

signals:
    void addTextToLogPTE(QString, LOG);
    void msgFromTcpToChess(QString, QString);
};

#endif // TCPMSGS_H

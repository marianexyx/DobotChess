#ifndef TCPMSGS_H
#define TCPMSGS_H

#include <QTcpSocket>
#include <QString>
#include "vars/log.h"
#include "vars/basic_vars.h"

struct TcpMsgMetadata
{
    int64_t ullTcpID;
    int nSender;
    QString QStrMsgForTcp;
};

class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;
    QList<TcpMsgMetadata> TCPMsgsList;

    bool m_bWaitingForReadyRead; //flaga dzięki której zablokujemy możliwość wykonywania od razu...
    //...zapytania z kolejki zapytań do TCP jeżeli aktualnie jest przetwarzane jakieś zapytanie...
    //...na którego odpowiedź(przetworzenie) z TCP czekamy
    int64_t m_ullID; //nr zapytania do tcp
    void doTcpConnect();

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

public slots:
    void TcpQueueMsg(int nSender, QString msg);

signals:
    void addTextToConsole(QString, LOG);
    void msgFromTcpToWeb(QString, QString);
    void msgFromTcpToArd(QString, QString);
    void msgFromTcpToCore(QString, QString);
};

#endif // TCPMSGS_H

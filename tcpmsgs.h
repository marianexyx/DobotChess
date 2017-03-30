#ifndef TCPMSGS_H
#define TCPMSGS_H

#include <QTcpSocket>
#include <QString>

struct TcpMsgMetadata
{
    unsigned long long ullTcpID;
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
    unsigned long long m_ullID; //nr zapytania do tcp
    void doTcpConnect();

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

    void queueMsgs(int nSender, QString msg);

public slots:
    void TcpQueueMsg(int nSender, QString msg);

signals:
    void addTextToConsole(QString, char);
    void msgFromTcpToWeb(QString, QString);
    void msgFromTcpToArd(QString, QString);
};

#endif // TCPMSGS_H

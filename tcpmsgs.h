#ifndef TCPMSGS_H
#define TCPMSGS_H

#include <QTcpSocket>
#include <QString>

struct TcpMsgMetadata
{
    int nSender;
    QString QStrMsgForTcp;
};


class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;
    QList<TcpMsgMetadata> TCPMsgsList;

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
    void addTextToTcpConsole(QString, char);
    void msgFromTcpToWeb(QString, QString);
    void msgFromTcpToArd(QString, QString);
};

#endif // TCPMSGS_H

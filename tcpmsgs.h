#ifndef TCPMSGS_H
#define TCPMSGS_H

#include <QTcpSocket>
#include <QString>

class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;

    QString _QStrMsgForChenard;
    QList<QString> TCPMsgsList;

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

    void queueMsgs(QString msgs);
    void doTcpConnect(QString QStrMsgForChenard);

signals:
    void addTextToTcpConsole(QString);
    void MsgFromChenard(QString);
};

#endif // TCPMSGS_H

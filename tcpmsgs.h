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

    void doTcpConnect(QString QStrMsgForChenard);

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

    void queueMsgs(QString msgs);

    bool bUndo; //TODO: do usunięcia na przyszłość po zmianie działania TCP

public slots:
    void TcpQueueMsg(QString msg);

signals:
    void addTextToTcpConsole(QString, char);
    void MsgFromChenard(QString);
};

#endif // TCPMSGS_H

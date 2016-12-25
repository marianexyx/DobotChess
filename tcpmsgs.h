#ifndef TCPMSGS_H
#define TCPMSGS_H

#include <QTcpSocket>
#include <QString>

class TCPMsgs: public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;

    bool _bBlokadaZapytan = false; // nie przyjmuj zapytań,dopóki poprzednie nie jest zrealizowane
    QString _QStrMsgForChenard;

private slots:
    //tcp
    void connected(); //sygnał i slot muszą mieć takie same przyjmowane argumenty
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

public:
    TCPMsgs();

    void doTcpConnect(QString QStrMsgForChenard);

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    bool getBlokadaZapytan() const    { return _bBlokadaZapytan; }

    void setBlokadaZapytan(bool bBlokadaZapytan)       { _bBlokadaZapytan = bBlokadaZapytan; }

signals:
    void addTextToTcpConsole(QString);
    void MsgFromChenard(QString);
};

#endif // TCPMSGS_H

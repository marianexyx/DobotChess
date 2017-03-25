#ifndef IGORBOT_H
#define IGORBOT_H

#include <QObject>
#include "chess.h"

class IgorBot: public QObject
{
    Q_OBJECT

private:
    Chessboard *_pChessboard;
    TCPMsgs *_pTCPMsgs;
    ArduinoUsb *_pArduinoUsb;
    Chess *_pChess;

    bool m_bAI;
    bool m_bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź na 'status' z tcp...
    //...dotyczy wykonanego ruchu gracza lub igora

    void wrongTcpAnswer(QString msgType, QString respond);
    void checkAI();

public:
    IgorBot(Chessboard *pChessboard, TCPMsgs *pTCPMsgs, ArduinoUsb *pArduinoUsb, Chess *pChes);

    void checkMsgFromChenard(QString msg);

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    void setAI(bool bAI)    { m_bAI = bAI; }
    bool getAI()            {return m_bAI; }

public slots:
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void enemyStart();
    QString enemySend(QString QsFT); //move from to
};

#endif // IGORBOT_H

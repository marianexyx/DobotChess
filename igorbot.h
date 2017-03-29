#ifndef IGORBOT_H
#define IGORBOT_H

#include "arduinousb.h"
#include "chess.h"

class IgorBot: public Chess
{
    Q_OBJECT

private:
    Dobot *_pDobot;
    Chessboard *_pChessboard;
    TCPMsgs *_pTCPMsgs;
    WebTable *_pWebTable;
    ArduinoUsb *_pArduinoUsb;

    bool m_bAI;
    bool m_bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    const int _nCommunicationType;

    void wrongTcpAnswer(QString msgType, QString respond);
    void checkAI();

public:
    IgorBot(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
            WebTable *pWebTable, ArduinoUsb *pArduinoUsb);

    //----------KOMUNIKACJA Z ARDUINO----------//
    void GameStarted();
    void BadMove(QString msg);
    void GameInProgress();
    void EndOfGame(QString msg);
    void PromoteToWhat();

    //----------KOMUNIKACJA Z CHENARD----------//
    void NewGame();
    void MoveTcpPiece(int type, QString msg);
    void Status(int sender);
    void Promote(QString msg);

    void Think5000();
    void UndoOk();
    void ThinkOk(QString msg);

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    void setAI(bool bAI)    { m_bAI = bAI; }
    bool getAI()            {return m_bAI; }

    /*---------------DZIEDZICZONE Z CHESS---------------//
    //---------------STEROWANIE RAMIENIEM---------------//
    void TestOk();
    void MoveOk(int nSender);
    void castlingMovingSequence();
    void enpassantMovingSequence();

    //-----------------FUNKCJE SZACHOWE-----------------//
    bool testEnpassant();
    bool testPromotion();
    void TestMove(QString QStrMsgFromWebsockets);

    //------KLASOWE POMOCNICZE METODY OBLICZENIOWE------//
    void wrongTcpAnswer(QString msgType, QString respond);*/

signals:
    //void addTextToConsole(QString, char); //jest dziedziczone?

public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msg);
    void EnemyStart(); //ogólna funkcja sterująca
};

#endif // IGORBOT_H

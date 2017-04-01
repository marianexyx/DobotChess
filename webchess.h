#ifndef WEBCHESS_H
#define WEBCHESS_H

#include "websockets.h"
#include "chess.h"

class WebChess : public Chess
{
    Q_OBJECT

 private:
    Websockets *_pWebsockets; //reszta wskaznikow dziedziczona 

public:
    WebChess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
             WebTable *pWebTable, Websockets *pWebsockets);

    //----------KOMUNIKACJA Z WWW----------//
    void GameStarted();
    void BadMove(QString msg);
    void GameInProgress();
    void EndOfGame(QString msg);
    void PromoteToWhat();

    //--------KOMUNIKACJA Z CHENARD--------//
    void NewGame();
    void MoveTcpPiece(QString msg);
    void Status(int sender);
    void Promote(QString msg);

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
    //void addTextToConsole(QString, char); //sygnał będę miał z klasy bazowej?

public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msgFromWs);
};

#endif // WEBCHESS_H

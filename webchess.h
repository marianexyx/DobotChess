#ifndef WEBCHESS_H
#define WEBCHESS_H

#include "websockets.h"
#include "chess.h"

class WebChess : public Chess
{
 private:
    Dobot *_pDobot;
    Chessboard *_pChessboard;
    TCPMsgs *_pTCPMsgs;
    WebTable *_pWebTable;
    Websockets *_pWebsockets;

    const int _nCommunicationType;

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
    void Promote(QString msg);
    void NewGame();
    void MoveTcpPiece(QString msg);
    void Status();

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
    void addTextToConsole(QString, char);

public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msgFromWs);
};

#endif // WEBCHESS_H

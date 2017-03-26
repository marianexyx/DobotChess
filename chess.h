#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QScrollBar>
#include <QDebug>
#include "dobot.h"
#include "chessboard.h"
#include "tcpmsgs.h"
#include "webtable.h"

class Chess: public QObject
{
    Q_OBJECT

private:
    TCPMsgs *_pTCPMsgs;
    Dobot *_pDobot;
    Chessboard *_pChessboard;
    WebTable *_pWebTable;

protected: //TODO: olać to przesłanianie wogle?

    bool _bServiceTests;
    const int _nCommunicationType;

    //----------------KOMUNIKACJA Z GRACZEM-------------//
    virtual void GameStarted();
    virtual void BadMove(QString QsMsgFromChenardTcp);
    virtual void GameInProgress();
    virtual void EndOfGame(QString QStrMsgFromChenardTcp);
    virtual void PromoteToWhat();

    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond); //protected slot?
    virtual void Promote(QString QStrMsgFromWs);
    virtual void checkMsgForChenard(QString msgFromWs); //protected slot?
    virtual void NewGame();
    virtual void MoveTcpPiece(int sender, QString msg);
    virtual void Status(int sender);

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
    void wrongTcpAnswer(QString msgType, QString respond);

public:
    Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs, WebTable *pWebTable);

    //--------STEROWANIE RAMIENIEM--------//
    void resetPiecePositions();
    void pieceMovingSequence(char chMoveType,
                                 int nPieceFromLetter = -1, int nPieceFromDigit = -1,
                                  int nPieceToLetter = -1, int nPieceToDigit = -1);

    //------METODY DOSTĘPOWE DO PÓL------//
    bool getServiceTests() const                { return _bServiceTests; }
    void setServiceTests(bool bServiceTests)    { _bServiceTests = bServiceTests; }

signals:
    void addTextToConsole(QString, char);
};

#endif // CHESS_H

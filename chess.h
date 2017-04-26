#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QScrollBar>
#include <QDebug>
#include <QRegularExpression>
#include "dobot.h"
#include "chessboard.h"
#include "tcpmsgs.h"
#include "webtable.h"

class Chess: public QObject
{
    Q_OBJECT

protected:
    TCPMsgs *_pTCPMsgs;
    Dobot *_pDobot;
    Chessboard *_pChessboard;
    WebTable *_pWebTable;

    bool _bServiceTests;

    //----------------KOMUNIKACJA Z GRACZEM-------------//
    virtual void GameStarted() = 0;
    virtual void BadMove(QString msg) = 0;
    virtual void GameInProgress() = 0;
    virtual void EndOfGame(QString msg) = 0;
    virtual void PromoteToWhat(QString moveForFuturePromote) = 0;

    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void NewGame() = 0;
    virtual void MoveTcpPiece( QString msg) = 0;
    virtual void Status() = 0;
    virtual void Promote(QString msg) = 0;
    virtual void AskForLegalMoves() = 0;

    //-----------------FUNKCJE SZACHOWE-----------------//
    virtual void TcpMoveOk() = 0;
    virtual void resetBoardCompleted() = 0;

    //---------------STEROWANIE RAMIENIEM---------------//
    void castlingMovingSequence();
    void enpassantMovingSequence();

    //-----------------FUNKCJE SZACHOWE-----------------//
    MOVE_TYPE checkMoveType(QString move);
    void handleMove(QString move);

    //------KLASOWE POMOCNICZE METODY OBLICZENIOWE------//
    void wrongTcpAnswer(QString msgType, QString respond);

public slots:
    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond) = 0; //protected slot?
    virtual void checkMsgForChenard(QString msg) = 0; //protected slot?

    //---------------STEROWANIE RAMIENIEM--------------//
    void resetPiecePositions();

public:
    Chess(); //czysto wirtualne klasy muszą mieć pusty konstruktor
    Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs, WebTable *pWebTable);

    //---------------STEROWANIE RAMIENIEM--------------//
    void pieceMovingSequence(MOVE_TYPE Type,
                             int nPieceFromLetter = -1, int nPieceFromDigit = -1,
                             int nPieceToLetter = -1, int nPieceToDigit = -1);

    //-------------METODY DOSTĘPOWE DO PÓL-------------//
    bool getServiceTests() const                { return _bServiceTests; }
    void setServiceTests(bool bServiceTests)    { _bServiceTests = bServiceTests; }

signals:
    void addTextToConsole(QString, char);
};

#endif // CHESS_H

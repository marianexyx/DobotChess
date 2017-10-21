#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QScrollBar>
#include <QDebug>
#include <QRegularExpression>
#include "dobot.h"
#include "chessboard.h"
#include "tcpmsgs.h"
#include "vars/board_axis.h"

//TODO: nie tworzyć dziedziczenia. tworzyć osobno obiekty z różnymi parametrami wejsciowymi (arduino/website)

class Chess: public QObject
{
    Q_OBJECT

protected:
    TCPMsgs *_pTCPMsgs;
    Dobot *_pDobot;
    Chessboard *_pChessboard;

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
    virtual void AskForHistoryMoves() = 0;
    void legalOk(QString msg);
    void historyOk(QString msg);

    //-----------------FUNKCJE SZACHOWE-----------------//
    virtual void TcpMoveOk() = 0;
    virtual void resetBoardCompleted() = 0;

    //---------------STEROWANIE RAMIENIEM---------------//
    void castlingMovingSequence();
    void enpassantMovingSequence();
    void goToSafeRemovedField(DIGIT digitTo, SEQUENCE_TYPE sequence);

    //-----------------FUNKCJE SZACHOWE-----------------//
    SEQUENCE_TYPE findMoveType(QString move);
    void handleMove(QString move);

    //------KLASOWE POMOCNICZE METODY OBLICZENIOWE------//
    void wrongTcpAnswer(QString msgType, QString respond);

public slots:
    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond) = 0; //protected slot?
    virtual void checkMsgForChenard(QString msg) = 0; //protected slot?

    //---------------STEROWANIE RAMIENIEM--------------//
    short findInitialPieceNrOnGivenField(short sField);
    short findInitialFieldOfGivenPiece(short sPiece);
    void resetPiecePositions();

public:
    Chess(); //czysto wirtualne klasy muszą mieć pusty konstruktor
    Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs);

    //---------------STEROWANIE RAMIENIEM--------------//
    void listMovesForDobot(SEQUENCE_TYPE Type,
                             LETTER pieceFromLetter = L_X, DIGIT pieceFromDigit = D_X,
                             LETTER pieceToLetter = L_X, DIGIT pieceToDigit = D_X);

signals:
    void addTextToConsole(QString, char);
};

#endif // CHESS_H

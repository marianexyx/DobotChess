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
#include "chess/chess_timers.h"
#include "chess/chess_movements.h"

//TODO: nie tworzyć dziedziczenia. tworzyć osobno obiekty z różnymi parametrami ...
//...wejsciowymi (arduino/website)- sprawdzić czy to da radę
/*todo2: na pierwszy rzut oka i po kolei powinno być widać to tu się dzieje:
ruch e2e4 wpada do obiektu szachow:
-sprawdzane jest ze statusu czy ruch był mozliwy
-określenie typu ruchu
-zakolejkowanie ruchu na dobocie
-wykonanie zapytania o ruch na tcp
-reagowanie na odpowiedzi z tcp*/

class Chess: public QObject
{
    Q_OBJECT

protected:
    TCPMsgs* _pTCPMsgs;
    Dobot* _pDobot;
    Chessboard* _pChessboard;
    Chessboard2* _pChessboardMain;
    Chessboard2* _pChessboardRemoved;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;

    PositionOnBoard _PieceFrom, _PieceTo;

    //----------------KOMUNIKACJA Z GRACZEM-------------//
    virtual void GameStarted() = 0;
    virtual void BadMove(QString msg) = 0;
    virtual void GameInProgress() = 0;
    virtual void EndOfGame(QString msg) = 0;
    virtual void PromoteToWhat(QString moveForFuturePromote) = 0;

    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void NewGame() = 0;
    //TODO: friend dla podklas, czy dziedziczyc? (tylko ze to virtual...)
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
    void movePieceWithManipulator(Chessboard2 *pRealBoard, PositionOnBoard FieldPos,
                                  VERTICAL_MOVE vertMove = VM_NONE);

    //------KLASOWE POMOCNICZE METODY OBLICZENIOWE------//
    void wrongTcpAnswer(QString msgType, QString respond);
    bool compareArrays(short nArray1[][8], short nArray2[][8]);

public slots:
    //--------------KOMUNIKACJA Z CHENARD--------------//
    virtual void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond) = 0; //protected slot?
    virtual void checkMsgForChenard(QString msg) = 0; //protected slot?

    //---------------STEROWANIE RAMIENIEM--------------//
    void resetPiecePositions();

public:
    Chess(); //czysto wirtualne klasy muszą mieć pusty konstruktor
    Chess(Dobot* pDobot, Chessboard* pChessboard, Chessboard2* pChessboardMain,
    Chessboard2* pChessboardRemoved, TCPMsgs* pTCPMsgs);


    //---------------STEROWANIE RAMIENIEM--------------//
    void listMovesForDobot(SEQUENCE_TYPE Type, //todo: rozdzielic na 2 ruchy
                             LETTER pieceFromLetter = L_X, DIGIT pieceFromDigit = D_X,
                             LETTER pieceToLetter = L_X, DIGIT pieceToDigit = D_X);

    PositionOnBoard getPieceFrom() const { return _PieceFrom; }
    PositionOnBoard getPieceTo() const { return _PieceTo; }


signals:
    void addTextToConsole(QString, char);
};

#endif // CHESS_H

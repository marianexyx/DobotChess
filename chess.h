#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QScrollBar>
#include <QDebug>
#include <QRegularExpression>
#include "dobot.h"
#include "chessboard.h"
#include "tcpmsgs.h"
#include "piece.h"
#include "vars/board_axis.h"
#include "vars/end_of_game_types.h"
#include "chess/chess_timers.h"
#include "chess/chess_movements.h"
#include "chess/chess_status.h"
#include "chess/chess_bot.h"

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

protected: //todo: private?
    TCPMsgs* _pTCPMsgs;
    Dobot* _pDobot;
    Chessboard* _pChessboard;
    Chessboard2* _pChessboardMain;
    Chessboard2* _pChessboardRemoved;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;
    ChessBot* _pBot;
    ChessStatus* _pStatus;

    Piece* _pPiece[32];

    COMMUNICATION_TYPES _PlayerSource;

    PositionOnBoard _PosFrom, _PosTo; //todo: przypisac wartosci
    QString QStrFuturePromote; //todo: zamienic na Pos x2? + nazwa malo adekwatna

    //----------------KOMUNIKACJA Z GRACZEM-------------//
    void GameInProgress();
    void SendDataToPlayer(QString msg);

    //--------------KOMUNIKACJA Z CHENARD--------------//
    void NewGame();
    //TODO: friend dla podklas, czy dziedziczyc? (tylko ze to virtual...)
    void Promote(QString msg);
    void SendMsgToTcp(QString msg);
    void legalOk(QString msg); //todo: czyli co sie dzieje dalej?
    void historyOk(QString msg); //todo: czyli co sie dzieje dalej?

    //-----------------FUNKCJE SZACHOWE-----------------//
    void TcpMoveOk(); //todo: czyli co sie dzieje dalej?
    void reset(); //todo: reset czego?
    void resetBoardCompleted(); //todo: czyli co sie dzieje dalej?

    //-----------------FUNKCJE SZACHOWE-----------------//
    SEQUENCE_TYPE findMoveType(QString move);
    void handleMove(QString move);
    void movePieceWithManipulator(Chessboard2 *pRealBoard, PositionOnBoard FieldPos,
                                  VERTICAL_MOVE vertMove = VM_NONE);

    //------METODY NARZEDZIOWE------//
    void wrongTcpAnswer(QString msgType, QString respond);
    void playerClickedStart(QString QStrWhoClicked); //todo: typ, a nie string
    bool compareArrays(short nArray1[][8], short nArray2[][8]);

public slots: //todo: protected slots?
    //--------------KOMUNIKACJA Z CHENARD--------------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msg);

    //---------------STEROWANIE RAMIENIEM--------------//
    void resetPiecePositions();

public:
    Chess(); //must have przy virualach
    Chess(Dobot* pDobot, Chessboard* pChessboard, Chessboard2* pChessboardMain,
    Chessboard2* pChessboardRemoved, TCPMsgs* pTCPMsgs);

    void promoteToWhat(QString moveForFuturePromote);
    void GameStarted();
    void BadMove(QString msg);
    void EndOfGame(QString msg);

    //---------------STEROWANIE RAMIENIEM--------------//
    void listMovesForDobot(SEQUENCE_TYPE Type, //todo: rozdzielic na 2 ruchy
                             LETTER pieceFromLetter = L_X, DIGIT pieceFromDigit = D_X,
                             LETTER pieceToLetter = L_X, DIGIT pieceToDigit = D_X);

    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }
    PositionOnBoard getPosFrom() const { return _PosFrom; }
    PositionOnBoard getPosTo() const { return _PosTo; }

    void setFuturePromote(QString QStrMove) { QStrFuturePromote = QStrMove; } //todo: friend
    void clearFuturePromote() { QStrFuturePromote.clear(); }
    QString getFuturePromote() const { return QStrFuturePromote; }

signals:
    void addTextToConsole(QString, LOG);
};

#endif // CHESS_H

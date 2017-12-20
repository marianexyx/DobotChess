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
#include "websockets.h"
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

protected: //todo: private/protected?
    Dobot* _pDobot;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Websockets* _pWebsockets;
    TCPMsgs* _pTCPMsgs;

    COMMUNICATION_TYPES _PlayerSource;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;
    ChessBot* _pBot;
    ChessStatus* _pStatus;

    Piece* _pPiece[32];

    //----------------KOMUNIKACJA Z GRACZEM-------------//
    void GameInProgress();
    void SendDataToPlayer(QString msg);

    //--------------KOMUNIKACJA Z CHENARD--------------//
    void NewGame();
    //TODO: friend dla podklas?
    void Promote(QString msg);
    void SendMsgToTcp(QString msg);

    //-----------------FUNKCJE SZACHOWE-----------------//
    void TcpMoveOk(); //todo: czyli co sie dzieje dalej?
    void reset(); //todo: reset czego?
    void resetBoardCompleted(); //todo: czyli co sie dzieje dalej?

    //-----------------FUNKCJE SZACHOWE-----------------//

    void handleMove(QString QStrMove); //todo: czyli co?
    void movePieceWithManipulator(Chessboard *pRealBoard, PosOnBoard FieldPos,
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
    Chess(Dobot* pDobot, Chessboard* pBoardMain, Chessboard* pBoardRemoved,
          TCPMsgs* pTCPMsgs, COMMUNICATION_TYPES PlayerSource);

    void GameStarted();
    void BadMove(QString msg);
    void EndOfGame(QString msg);
    void resetBoardData();
    bool isPiecesSetOk();

    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }

signals:
    void addTextToConsole(QString, LOG);
};

#endif // CHESS_H

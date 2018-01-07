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
#include "arduinousb.h"
#include "client.h"
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
//todo: szachy nie powinny raczej wiedzieć nic o graczu, który steruje grą

class Chess: public QObject
{
    Q_OBJECT

private:
    Clients* _pClients;
    Dobot* _pDobot;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Websockets* _pWebsockets; //todo: da się z tej klasy pozbyć websocketow
    TCPMsgs* _pTCPMsgs;
    ArduinoUsb* _pUsb;

    COMMUNICATION_TYPES _PlayerSource;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;
    ChessBot* _pBot;
    ChessStatus* _pStatus;

    Piece* _pPiece[32];

    void startNewGameInChenard();
    void continueGameplay();
    void sendMsgToTcp(QString msg);
    void restoreGameToInitialState();
    void resetBoardCompleted(); //todo: czyli co sie dzieje dalej?
    void movePieceWithManipulator(Chessboard *pRealBoard, PosOnBoard FieldPos,
                                  VERTICAL_MOVE vertMove = VM_NONE);
    void wrongTcpAnswer(QString msgType, QString respond);
    void playerClickedStart(PLAYER_TYPE playerType);
    bool compareArrays(short nArray1[][8], short nArray2[][8]); //todo: do basic vars

public:
    Chess(Clients* pClients, Dobot* pDobot, Chessboard* pBoardMain,
          Chessboard* pBoardRemoved, ArduinoUsb* pUsb, Websockets* pWebsockets,
          TCPMsgs* pTCPMsgs, COMMUNICATION_TYPES PlayerSource);
    ~Chess();

    void findAndSaveMoveAndSendItToTcp(QString QStrMove);
    void GameStarted();
    void tellPlayerThatHeGaveBadMove(QString msg);
    void EndOfGame(QString msg, Client playerToClear);
    void resetBoardData();
    bool isPiecesSetOk();
    bool isPieceStayOnItsStartingField(short sPieceNr); //todo: wskaznik do piece?
    void removeClient(int64_t clientID); //todo: taka sama nazwa jak w cliencie. zmienić coś
    void playerIsLeavingGame(Client player, END_TYPE LeavingType); //todo: websocket param
    void endOfGame(END_TYPE EndType, Client playerToClear /*= nullptr*/); //todo: websocket param

    PLAYER_TYPE getActivePlayerType();
    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }

    //todo: friends to rest of ingedients
    ChessTimers* getTimersPointer() { return _pTimers; }
    ChessMovements* getMovementsPointer() { return _pMovements; }
    ChessBot* getBotPointer() { return _pBot; }
    ChessStatus* getStatusPointer() { return _pStatus; }

public slots:
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgFromWebsockets(QString msg, int64_t clientID);
    void sendDataToClient(QString msg, int64_t clientID = -1);
    void resetPiecePositions();
    QString getTableData();

signals:
    void addTextToConsole(QString, LOG);
};

#endif // CHESS_H

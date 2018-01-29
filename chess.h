#ifndef CHESS_H
#define CHESS_H

#pragma once
#include <QScrollBar>
#include <QRegularExpression>
#include "dobot.h"
#include "chessboard.h"
#include "tcpmsgs.h"
#include "websockets.h"
#include "arduinousb.h"
#include "vars/game_status.h"
#include "chess/chess_timers.h"
#include "chess/chess_movements.h"
#include "chess/chess_status.h"
#include "chess/chess_bot.h"
#include "chess/chess_resets.h"
#include "chess/chess_conditions.h"

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

private:
    Clients* _pClients;
    Dobot* _pDobot;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Websockets* _pWebsockets;
    TCPMsgs* _pTCPMsgs;
    ArduinoUsb* _pUsb;

    COMMUNICATION_TYPE _PlayerSource;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;
    ChessBot* _pBot;
    ChessStatus* _pStatus;
    ChessResets* _pResets;
    ChessConditions* _pConditions;

    Piece* _pPiece[32];

    GAME_STATUS _ChessGameStatus;
    clientRequest _request;

    void coreIsReadyForNewGame();
    void playerWantToStartNewGame(PLAYER_TYPE playerType);
    void startNewGameInChenard(bool bService = false);
    void continueGameplay();
    void sendMsgToTcp(QString QStrMsg);

public:
    Chess(Clients* pClients, Dobot* pDobot, Chessboard* pBoardMain,
          Chessboard* pBoardRemoved, ArduinoUsb* pUsb, Websockets* pWebsockets,
          TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource);
    ~Chess();

    void sendDataToClient(QString QStrMsg, Client* pClient = nullptr);
    void sendDataToAllClients(QString QStrMsg);

    void findAndSaveMoveAndSendItToTcp(QString QStrMove);
    void startNewGameInCore();
    void tellPlayerThatHeGaveBadMove(QString QStrMsg);
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece);
    Field* searchForPieceActualFieldOnMainBoard(Piece* pPiece);
    void removeClient(Client* pClient);
    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE vertMove = VM_NONE);
    GAME_STATUS getGameStatus() const { return _ChessGameStatus; }

    PLAYER_TYPE getActivePlayerType();
    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }

    //todo: friends to rest of ingredients
    Clients* getClientsPointer() { return _pClients; }
    Chessboard* getBoardMainPointer() { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() { return _pBoardRemoved; }

    ChessTimers* getTimersPointer() { return _pTimers; }
    ChessMovements* getMovementsPointer() { return _pMovements; }
    ChessBot* getBotPointer() { return _pBot; }
    ChessStatus* getStatusPointer() { return _pStatus; }
    ChessResets* getResetsPointer() { return _pResets; }
    ChessConditions* getConditionsPointer() { return _pConditions; }

public slots:
    void checkMsgFromWebsockets(QString QStrMsg, Client* pClient);
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgFromUsb(QString QStrMsg);
    QString getTableData();

signals:
    void addTextToLogPTE(QString, LOG);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showLegalMoves(QStringList);
    void showHistoryMoves(QStringList);
};

#endif // CHESS_H

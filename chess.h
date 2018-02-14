#ifndef CHESS_H
#define CHESS_H

#pragma once
#include <QScrollBar>
#include <QRegularExpression>
#include "dobot.h"
#include "vars/board_axis.h"
#include "vars/board_data_labels.h"
#include "vars/board_types.h"
#include "tcpmsgs.h"
#include "websockets.h"
#include "chess/game_status.h"
#include "chess/chenard_io_msgs.h"
#include "chess/sequence_types.h"
#include "chess/end_of_game_types.h"
#include "chessboard.h"
#include "chess/chess_bot.h"
#include "chess/chess_movements.h"
#include "chess/chess_timers.h"
#include "chess/chess_resets.h"
#include "chess/chess_conditions.h"
#include "chess/chess_status.h"

class ChessBot;

class Chess: public QObject
{
    Q_OBJECT

    friend class ChessBot;
    friend class ChessTimers;
    friend class ChessMovements;
    friend class ChessStatus;
    friend class ChessResets;
    friend class ChessConditions;

private:
    Clients* _pClientsList;
    Dobot* _pDobot;
    Chessboard* _pBoardMain;
    Chessboard* _pBoardRemoved;
    Chessboard* _pBoardChenard;
    Websockets* _pWebsockets;
    TCPMsgs* _pTCPMsgs;
    ArduinoUsb* _pUsb;

    COMMUNICATION_TYPE _PlayerSource;

    ChessBot* _pBot;
    ChessTimers* _pTimers;
    ChessStatus* _pStatus;
    ChessResets* _pResets;
    ChessConditions* _pConditions;
    ChessMovements* _pMovements;

    Piece* _pPiece[32];

    GAME_STATUS _ChessGameStatus; //todo: wyświetlać w pętli w tytule okna stan gry
    clientRequest _request;

    //communication methods
    void playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService = false);
    void tellPlayerThatHeGaveBadMove(QString QStrMsg);
    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE VertMove = VM_NONE);
    void sendMsgToTcp(QString QStrMsg);
    void newClientName(Client* pClient, clientRequest request);
    void removeClient(Client* pClient);
    void sendDataToClient(QString QStrMsg, Client* pClient = nullptr);
    void sendDataToAllClients(QString QStrMsg);

    //gameplay methods
    void coreIsReadyForNewGame();
    void startNewGameInCore();
    void manageMoveRequest(clientRequest request);
    void continueGameplay();
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece);
    Field* searchForPieceActualFieldOnMainBoard(Piece* pPiece);

public:
    Chess(Clients* pClientsList, Dobot* pDobot, Chessboard* pBoardMain,
          Chessboard* pBoardRemoved, Chessboard* pBoardChenard, Websockets* pWebsockets,
          TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource);
    ~Chess();

    void setGameStatus(GAME_STATUS Status) { _ChessGameStatus = Status; }

    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }

    //todo: friends to rest of ingredients. jeżeli cała klasa to friend, to nie potrzebuje...
    //...poniższych funkcji
    Clients* getClientsPointer() { return _pClientsList; }
    Chessboard* getBoardMainPointer() { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() { return _pBoardRemoved; }
    ArduinoUsb* getUsbPointer() { return _pUsb; }

    ChessTimers* getTimersPointer() { return _pTimers; }
    ChessMovements* getMovementsPointer() { return _pMovements; }
    ChessBot* getBotPointer() { return _pBot; }
    ChessStatus* getStatusPointer() { return _pStatus; }
    ChessResets* getResetsPointer() { return _pResets; }
    ChessConditions* getConditionsPointer() { return _pConditions; }

    GAME_STATUS getGameStatus() const { return _ChessGameStatus; }
    clientRequest getLastRequest() const { return _request; }

public slots:
    void checkMsgFromWebsockets(QString QStrMsg, Client* pClient);
    void checkMsgFromChenard(QString QStrTcpMsgType, QString QStrTcpRespond);
    void checkMsgFromUsb(QString QStrMsg);
    QString getTableData();

signals:
    void addTextToLogPTE(QString, LOG);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showLegalMovesInUI(QStringList);
    void showHistoryMovesInUI(QStringList);
};

#endif // CHESS_H

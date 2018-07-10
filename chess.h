#ifndef CHESS_H
#define CHESS_H

#pragma once
#include <QScrollBar>
#include <QRegularExpression>
#include "vars/action_types.h"
#include "vars/board_axis.h"
#include "vars/board_data_labels.h"
#include "vars/board_types.h"
#include "vars/table_data_types.h"
#include "tcpmsgs.h"
#include "websockets.h"
#include "chess/vars/game_status.h"
#include "chess/vars/chenard_io_msgs.h"
#include "chess/vars/sequence_types.h"
#include "chess/vars/end_of_game_types.h"
#include "piece_set.h"
#include "chessboard.h"
#include "chess/chess_bot.h"
#include "chess/chess_movements.h"
#include "chess/chess_timers.h"
#include "chess/chess_conditions.h" //must be as last chess...
//...part. chess_status.h included here
#include "dobot.h"

class Chess: public QObject
{
    Q_OBJECT

private:
    Clients* _pClientsList;
    Dobot* _pDobot;
    PieceController* _pPieceController;
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
    ChessConditions* _pConditions;
    ChessMovements* _pMovements;

    GAME_STATUS _ChessGameStatus;
    clientRequest _request;

    //future: those 2 blocks below can be divided into classes...
    //...f.e.: game+communication

    //communication methods
    void playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService = false);
    void tellPlayerThatHeGaveBadMove(QString QStrMsg);
    void sendMsgToTcp(QString QStrMsg);
    void newClientLogged(Client& client, int64_t sqlID);
    void restorateGameIfDisconnectedClientAffectIt(Client& client);
    void sendDataToClient(QString QStrMsg, Client client = Client());
    void sendDataToAllClients(QString QStrMsg);
    QString getEndGameMsg(END_TYPE WhoWon, QString QStrTableData,
                          PLAYER_TYPE PlayerToClear = PT_NONE);
    void updateClientsInUI();

    //gameplay methods
    void coreIsReadyForNewGame();
    void startNewGameInCore();
    void manageMoveRequest(clientRequest request);
    void continueGameplay();
    void restartGame(END_TYPE WhoWon, PLAYER_TYPE PlayerToClear = PT_NONE);
    void changePlayersOnChairs();
    void resetTableData();

public:
    Chess(PieceController* pPieceController, Chessboard* pBoardChenard,
          Websockets* pWebsockets, TCPMsgs* pTCPMsgs,
          COMMUNICATION_TYPE PlayerSource);
    ~Chess();

    void setGameStatus(GAME_STATUS Status) { _ChessGameStatus = Status; }

    //todo: friends to rest of ingredients. if whole class is friend, then getters...
    //...below are useless
    GAME_STATUS getGameStatus() const { return _ChessGameStatus; }
    clientRequest getLastRequest() const { return _request; }

    Clients* getClientsPointer() const { return _pClientsList; }
    Dobot* getDobotPointer() const { return _pDobot; }
    Chessboard* getBoardMainPointer() const { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() const { return _pBoardRemoved; }
    Chessboard* getBoardChenardPointer() const { return _pBoardChenard; }
    Websockets* getWebsocketsPointer() const { return _pWebsockets; }
    PieceController* getPieceControllerPointer() const { return _pPieceController; }
    TCPMsgs* getTCPMsgsPointer() const { return _pTCPMsgs; }

    ChessTimers* getTimersPointer() const { return _pTimers; }
    ChessMovements* getMovementsPointer() const { return _pMovements; }
    ChessBot* getBotPointer() const { return _pBot; }
    ChessStatus* getStatusPointer() const { return _pStatus; }
    ChessConditions* getConditionsPointer() const { return _pConditions; }

public slots:
    void checkMsgFromWebsockets(QString QStrMsg, int64_t n64SenderID);
    void checkMsgFromChenard(QString QStrTcpMsgType, QString QStrTcpRespond);
    void checkMsgFromUsb(QString QStrMsg);
    QString getTableData();
    void setBoardDataLabelInUI(QString QStrLabel, BOARD_DATA_LABEL LabelType);
    void showLegalMovesInForm(QStringList legalMoves);
    void showHistoryMovesInForm(QStringList historyMoves);
    void timeOutStart();
    void timeOutPlayer(PLAYER_TYPE Player);

signals:
    void addTextToLogPTE(QString, LOG);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showLegalMovesInUI(QStringList);
    void showHistoryMovesInUI(QStringList);
};

#endif // CHESS_H

#ifndef CHESS_H
#define CHESS_H

#pragma once
#include <QScrollBar>
#include <QRegularExpression>
#include "vars/board_data_labels.h"
#include "vars/table_data_types.h"
#include "tcpmsgs.h"
#include "websockets.h"
#include "chess/vars/action_types.h"
#include "chess/vars/game_status.h"
#include "chess/vars/chenard_io_msgs.h"
#include "chess/vars/sequence_types.h"
#include "chess/vars/end_of_game_types.h"
#include "piece_controller.h"
#include "chess/chess_movements.h"
#include "chess/chess_timers.h"
#include "chess/chess_conditions.h" //must be included as last chess...
//...file, because chess_status.h is included in this one
#include "dobot/dobot.h"

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
    void sendDataToClient(Client client, ACTION_TYPE AT = AT_NONE,
                          END_TYPE ET = ET_NONE);
    void sendDataToAllClients(ACTION_TYPE AT = AT_NONE, END_TYPE ET = ET_NONE);
    void updateClientsInUI();

    //gameplay methods
    void makeCoreReadyForNewGame();
    void startNewGameInCore();
    void manageMoveRequest(clientRequest request);
    void continueGameplay();
    void restartGame(END_TYPE ET);
    void changePlayersOnChairs();
    void playerLeftChair(PLAYER_TYPE PT);
    void fillTableWithNextQueuedClientsIfTheyExist();
    void restorateGameIfDisconnectedClientAffectIt(Client& client);
    void resetTableData();

public:
    Chess(PieceController* pPieceController, Chessboard* pBoardChenard,
          Websockets* pWebsockets, TCPMsgs* pTCPMsgs);
    ~Chess();

    void setGameStatus(GAME_STATUS Status) { _ChessGameStatus = Status; }
    void clearLastSavedMadeMove() { _pStatus->clearMove(); }

    GAME_STATUS getGameStatus() const { return _ChessGameStatus; }
    Clients* getClientsPointer() const { return _pClientsList; }
    Dobot* getDobotPointer() const { return _pDobot; }
    Chessboard* getBoardMainPointer() const { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() const { return _pBoardRemoved; }
    Chessboard* getBoardChenardPointer() const { return _pBoardChenard; }
    Websockets* getWebsocketsPointer() const { return _pWebsockets; }
    PieceController* getPieceControllerPointer() const { return _pPieceController; }
    TCPMsgs* getTCPMsgsPointer() const { return _pTCPMsgs; }
    QString dumpAllData();

public slots:
    void checkMsgFromWebsockets(QString QStrMsg, int64_t n64SenderID, bool bService = false);
    void checkMsgFromChenard(QString QStrTcpMsgType, QString QStrTcpRespond);
    QString getTableData(ACTION_TYPE AT = AT_NONE, END_TYPE ET = ET_NONE);
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
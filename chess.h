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
#include "chess/vars/game_status.h"
#include "chess/vars/chenard_io_msgs.h"
#include "chess/vars/sequence_types.h"
#include "chess/vars/end_of_game_types.h"
#include "piece_set.h"
#include "chessboard.h"
#include "chess/chess_bot.h"
#include "chess/chess_movements.h"
#include "chess/chess_timers.h"
#include "chess/chess_conditions.h" //must be last. chess_status.h included here

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

    GAME_STATUS _ChessGameStatus; //todo: wyświetlać w pętli w tytule okna stan gry
    clientRequest _request;

    //communication methods
    void playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService = false);
    void tellPlayerThatHeGaveBadMove(QString QStrMsg);
    void sendMsgToTcp(QString QStrMsg);
    void newClientName(Client& client, clientRequest request);
    void removeClientFromList(Client& client);
    void sendDataToClient(QString QStrMsg, Client client = Client());
    void sendDataToAllClients(QString QStrMsg);
    QString getEndGameMsg(END_TYPE WhoWon, QString QStrTableData,
                          PLAYER_TYPE PlayerToClear = PT_NONE);

    //gameplay methods
    void coreIsReadyForNewGame();
    void startNewGameInCore();
    void manageMoveRequest(clientRequest request);
    void continueGameplay();
    void restartGame(END_TYPE WhoWon, PLAYER_TYPE PlayerToClear = PT_NONE);
    void changePlayersOnChairs(END_TYPE WhoWon, PLAYER_TYPE PlayerToClear);

public:
    Chess(Clients* pClientsList, Dobot* pDobot, PieceController* pPieceController,
          Chessboard* pBoardMain, Chessboard* pBoardRemoved, Chessboard* pBoardChenard,
          Websockets* pWebsockets, TCPMsgs* pTCPMsgs, COMMUNICATION_TYPE PlayerSource);
    ~Chess();

    void setGameStatus(GAME_STATUS Status) { _ChessGameStatus = Status; }

    //todo: friends to rest of ingredients. jeżeli cała klasa to friend, to nie potrzebuje...
    //...poniższych funkcji

    GAME_STATUS getGameStatus() const { return _ChessGameStatus; }
    clientRequest getLastRequest() const { return _request; }

    Clients* getClientsPointer() { return _pClientsList; }
    Chessboard* getBoardMainPointer() { return _pBoardMain; }
    Chessboard* getBoardRemovedPointer() { return _pBoardRemoved; }

    ChessTimers* getTimersPointer() { return _pTimers; }
    ChessMovements* getMovementsPointer() { return _pMovements; }
    ChessBot* getBotPointer() { return _pBot; }
    ChessStatus* getStatusPointer() { return _pStatus; }
    ChessConditions* getConditionsPointer() { return _pConditions; }

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

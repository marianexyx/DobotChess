#ifndef CHESS_H
#define CHESS_H

#pragma once
#include <QScrollBar>
#include <QRegularExpression>
#include "vars/board_data_labels.h"
#include "vars/table_data_types.h"
#include "tcpmsgs.h"
#include "websockets.h"
#include "arduino_usb.h"
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
    Clients* m_pClientsList;
    Dobot* m_pDobot;
    PieceController* m_pPieceController;
    Chessboard* m_pBoardMain;
    Chessboard* m_pBoardRemoved;
    Chessboard* m_pBoardChenard;
    Websockets* m_pWebsockets;
    TCPMsgs* m_pTCPMsgs;
    ArduinoUsb* m_pUSB;

    ChessTimers* m_pTimers;
    ChessStatus* m_pStatus;
    ChessConditions* m_pConditions;
    ChessMovements* m_pMovements;

    GAME_STATUS m_ChessGameStatus;
    QList<clientRequest> m_requestList;
    bool m_bExecutingClientRequest;
    QTimer* m_executingClientRequestTimer;
    QTimer* m_keepConnectedTimer;

    //future: those 2 blocks below can be divided into classes (game+communication). game...
    //...methods are mosty included in executeClientRequest() method (check it). ...
    //...chess class will be small compared to communication class

    //communication methods
    void sendDataToClient(Client client, ACTION_TYPE AT = AT_NONE, END_TYPE ET = ET_NONE);
    void sendDataToAllClients(ACTION_TYPE AT = AT_NONE, END_TYPE ET = ET_NONE);
    void sendMsgToChessEngine(QString QStrMsg);
    void killClient(const Client &client, REJECTED_REQUEST_REACTION RRR);
    void startExecutingClientsRequests();
    void closeClientRequest();
    void executeClientRequest(clientRequest request);
    bool playerWantToStartNewGame(PLAYER_TYPE PlayerType, bool bService = false);
    bool manageMoveRequest(clientRequest request);
    void updateClientsInUI();

    //gameplay methods
    void startNewGameInCore();
    void restartGame(END_TYPE ET);
    void resetTableData();
    void changePlayersOnChairs();
    void fillTableWithNextQueuedClientsIfTheyExist();
    void makeCoreReadyForNewGame();
    void continueGameplay();
    void restorateGameIfDisconnectedClientAffectIt(const Client& clientToDisconnect);
    void playerLeftChair(PLAYER_TYPE PT);

private slots:
    void clientRequesTimeOut();
    void keepConnectedTimeOut();

public:
    Chess(PieceController* pPieceController);
    ~Chess();

    void setGameStatus(GAME_STATUS Status) { m_ChessGameStatus = Status; }
    void clearLastSavedMadeMove() { m_pStatus->clearMove(); }
    QString dumpAllData();

    GAME_STATUS getGameStatus() const { return m_ChessGameStatus; }
    WHOSE_TURN getWhoseTurn() const { return m_pStatus->getWhoseTurn(); }

    Clients* getClientsPointer() const { return m_pClientsList; }
    Dobot* getDobotPointer() const { return m_pDobot; }
    Chessboard* getBoardMainPointer() const { return m_pBoardMain; }
    Chessboard* getBoardRemovedPointer() const { return m_pBoardRemoved; }
    Chessboard* getBoardChenardPointer() const { return m_pBoardChenard; }
    Websockets* getWebsocketsPointer() const { return m_pWebsockets; }
    PieceController* getPieceControllerPointer() const { return m_pPieceController; }
    TCPMsgs* getTCPMsgsPointer() const { return m_pTCPMsgs; }
    ArduinoUsb* getUSBPointer() const { return m_pUSB; }

public slots:
    void checkMsgFromClient(QString QStrMsg, uint64_t un64SenderID, bool bService = false);
    void checkMsgFromChessEngine(QString QStrTcpMsgType, QString QStrTcpRespond);
    void checkMsgFromUSB(QString QStrMsg);
    void timeOutStart();
    void timeOutPlayer(PLAYER_TYPE Player);
    QString getTableData(ACTION_TYPE AT = AT_NONE, END_TYPE ET = ET_NONE);
    //future: standardize GUI slots names: UI/form
    void setBoardDataLabelInUI(QString QStrLabel, BOARD_DATA_LABEL LabelType);
    void showLegalMovesInForm(QStringList legalMoves);
    void showHistoryMovesInForm(QStringList historyMoves);
    void updatePortsComboBoxInUI(int nPorts);

signals:
    void addTextToLogPTE(QString, LOG);
    void setBoardDataLabel(QString, BOARD_DATA_LABEL);
    void showLegalMovesInUI(QStringList);
    void showHistoryMovesInUI(QStringList);
    void updatePortsComboBox(int);
};

#endif // CHESS_H

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
#include "chess/chess_resets.h"

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
    Websockets* _pWebsockets;
    TCPMsgs* _pTCPMsgs;
    ArduinoUsb* _pUsb;

    COMMUNICATION_TYPES _PlayerSource;

    ChessTimers* _pTimers;
    ChessMovements* _pMovements;
    ChessBot* _pBot;
    ChessStatus* _pStatus;
    ChessResets _pResets;

    Piece* _pPiece[32];

    void coreIsReadyForNewGame();
    void playerWantToStartNewGame(PLAYER_TYPE playerType);
    void startNewGameInChenard(bool bService = false);
    void continueGameplay();
    void sendMsgToTcp(QString QStrMsg);
    void wrongTcpAnswer(QString msgType, QString respond); 

public:
    Chess(Clients* pClients, Dobot* pDobot, Chessboard* pBoardMain,
          Chessboard* pBoardRemoved, ArduinoUsb* pUsb, Websockets* pWebsockets,
          TCPMsgs* pTCPMsgs, COMMUNICATION_TYPES PlayerSource);
    ~Chess();

    void sendDataToClient(QString QStrMsg, Client* pClient = nullptr);
    void sendDataToAllClients(QString QStrMsg);

    void findAndSaveMoveAndSendItToTcp(QString QStrMove);
    void gameStarted(); //todo: czyli co?
    void tellPlayerThatHeGaveBadMove(QString QStrMsg);
    bool isPieceSetOk();
    bool isPieceStayOnItsStartingField(Piece* pPiece);
    Field* searchForPieceActualFieldOnMainBoard(Piece* pPiece);
    bool isPieceSetOnBoardsIdentical(Chessboard* pBoard1, Chessboard* pBoard2);
    void removeClient(Client* pClient);
    void movePieceWithManipulator(Chessboard *pRealBoard, Field* pField,
                                  VERTICAL_MOVE vertMove = VM_NONE);

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

public slots:
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgFromWebsockets(QString QStrMsg, Client* pClient);
    QString getTableData();

signals:
    void addTextToConsole(QString, LOG);
};

#endif // CHESS_H

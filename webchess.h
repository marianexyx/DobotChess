#ifndef WEBCHESS_H
#define WEBCHESS_H

#pragma once

#include "websockets.h"
#include "chess.h"
#include "vars/basic_vars.h"

class WebChess : public Chess
{
    Q_OBJECT

 private:
    Websockets *_pWebsockets; //reszta wskaznikow dziedziczona 

public:
    WebChess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs, Websockets *pWebsockets);

    //----------KOMUNIKACJA Z WWW----------//
    void GameStarted();
    void BadMove(QString msg);
    void GameInProgress();
    void EndOfGame(QString msg);
    void PromoteToWhat(QString moveForFuturePromote);

    //--------KOMUNIKACJA Z CHENARD--------//
    void NewGame();
    void MoveTcpPiece(QString msg);
    void Status();
    void Promote(QString msg);
    void AskForLegalMoves();

    //-----------------FUNKCJE WEWNĘTRZNE-----------------//
    void TcpMoveOk();
    SEQUENCE_TYPE checkMoveType(QString move);
    void reset();
    void resetBoardCompleted();
    void playerClickedStart(QString QStrWhoClicked);

signals:
    void addTextToConsole(QString, LOG);

public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msgFromWs);
};

#endif // WEBCHESS_H

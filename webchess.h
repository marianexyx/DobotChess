#ifndef WEBCHESS_H
#define WEBCHESS_H

#include "websockets.h"
#include "chess.h"

class WebChess : public Chess
{
    Q_OBJECT

 private:
    Websockets *_pWebsockets; //reszta wskaznikow dziedziczona 

public:
    WebChess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
             WebTable *pWebTable, Websockets *pWebsockets);

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

    void legalOk(QString msg);

    //-----------------FUNKCJE SZACHOWE-----------------//
    void TcpMoveOk();
    MOVE_TYPE checkMoveType(QString move);
     void resetBoardCompleted();

signals:
    //void addTextToConsole(QString, char); //TODO: sygnał będę miał z klasy bazowej?

public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void checkMsgForChenard(QString msgFromWs);
};

#endif // WEBCHESS_H

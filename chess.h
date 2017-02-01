#ifndef CHESS_H
#define CHESS_H

#include <QString>
#include <QScrollBar>
#include <QDebug>
#include "dobot.h"
#include "chessboard.h"
#include "websockets.h"
#include "tcpmsgs.h"
//#include "webtable.h"

class Chess: public QObject
{
    Q_OBJECT

private:
    TCPMsgs *_pTCPMsgs;
    Dobot *_pDobot;
    Chessboard *_pChessboard;
    Websockets *_pWebsockets;
    WebTable *_pWebTable;

    bool _bServiceTests;

    bool b_test_enpassant = false;	//zmienna odpowiadająca za wykonywanie ruchu enpassant
    QString QS_enpassantToReject;   //zmienna: pozycja pionka bitego w enpassant

    QString QS_futurePromote = "";  //zmienna pamiętająca jakie było zapytanie o ruch...
    //...typu promocja (pójdzie drugie zapytanie i potrzeba będzie stare skądś odczytać).
    bool bPromotionConfirmed = false; //gdy podczas promocji pojawi się zbijanie, to dzięki tej...
    //...zmiennej program wie co i jak zbijać podczas ruchu typu promocja.

    void normalPieceMovingSequence();
    void removePieceSequence();
    void castlingMovingSequence(QString QS_msgFromSerial);
    void enpassantMovingSequence();
    bool testEnpassant();
    bool testPromotion();

    void MovePiece(QString QStrMsgFromWebsockets);
    void NewGame();
    void MoveOk();
    void Promote(QString QStrMsgFromWebsockets);
    void Illegal();
    void GameStarted();
    void TestOk();
    void GameInProgress();
    void EndOfGame(QString QStrMsgFromChenardTcp);
    void BadMove(QString QsMsgFromChenardTcp);

public:
    Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
          Websockets *pWebsockets, WebTable *pWebTable);

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    bool getServiceTests() const                { return _bServiceTests; }

    void setServiceTests(bool bServiceTests)    { _bServiceTests = bServiceTests; }

public slots:
    void checkMsgFromChenard(QString QStrMsgFromChenardTcp);
    void checkMsgFromWebsockets(QString QStrMsgFromWebsockets); //TODO: podpiąć wszędzie w ...
    //...klasie websocketów. TODO2: wyzwalać tutaj pojemnik na komendy do tcp.

signals:
    void addTextToDobotConsole(QString QS_msg); //dodawanie komunikatu do konsoli dobota

};

#endif // CHESS_H

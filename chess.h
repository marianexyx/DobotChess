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
    bool m_bAI;

    /*void pieceMovingSequence(char chMoveType,
                             int nPieceFromLetter = -1, int nPieceFromDigit = -1,
                              int nPieceToLetter = -1, int nPieceToDigit = -1);*/ //tymczasowy public
    void castlingMovingSequence();
    void enpassantMovingSequence();
    bool testEnpassant();
    bool testPromotion();

    void TestMove(QString QStrMsgFromWebsockets);
    void MovePiece(QString QStrMsgFromWebsockets);
    void MoveOk();
    void Promote(QString QStrMsgFromWs);
    void GameStarted();
    void TestOk();
    void GameInProgress();
    void EndOfGame(QString QStrMsgFromChenardTcp);
    void BadMove(QString QsMsgFromChenardTcp);

public:
    Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
          Websockets *pWebsockets, WebTable *pWebTable);

    void NewGame(); //TODO: przy tych 2 funkcjach mam problem z dostępem z poziomu mainwindow...
    void resetPiecePositions(); //...do nich. Były one jako prywatne i tam w sumie winny zostać jakoś.
    void pieceMovingSequence(char chMoveType,
                                 int nPieceFromLetter = -1, int nPieceFromDigit = -1,
                                  int nPieceToLetter = -1, int nPieceToDigit = -1);

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    bool getServiceTests() const                { return _bServiceTests; }
    bool getAI() const                          { return m_bAI; }

    void setServiceTests(bool bServiceTests)    { _bServiceTests = bServiceTests; }
    void setAI(bool bAI)    { m_bAI = bAI; }

public slots:
    void checkMsgFromChenard(QString tcpRespond);
    void checkMsgFromWebsockets(QString msgFromWs);

signals:
    void addTextToDobotConsole(QString, char); //dodawanie komunikatu do konsoli dobota
    void addTextToCoreConsole(QString,char); //TODO: może się przydać jak upiekszę kod

};

#endif // CHESS_H

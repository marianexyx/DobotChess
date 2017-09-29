#ifndef IGORBOT_H
#define IGORBOT_H

#include "arduinousb.h"
#include "chess.h"
#include "vars/basic_vars.h"

class IgorBot: public Chess
{
    Q_OBJECT

private:
    ArduinoUsb *_pArduinoUsb; //reszta wskaznikow dziedziczona

    bool m_bAI;
    bool m_bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    bool m_bIsIgorsAiSimulatedAsPlayer2;

    void wrongTcpAnswer(QString msgType, QString respond);
    void checkAI();

public:
    IgorBot(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs, ArduinoUsb *pArduinoUsb);

    //----------KOMUNIKACJA Z ARDUINO----------//
    void GameStarted();
    void BadMove(QString msg);
    void GameInProgress();
    void EndOfGame(QString msg);
    void PromoteToWhat(QString moveForFuturePromote);

    //----------KOMUNIKACJA Z CHENARD----------//
    void NewGame();
    void MoveTcpPiece(QString msg);
    void Status();
    void Promote(QString msg);
    void AskForLegalMoves();
    void AskForHistoryMoves();

    void Think5000();
    void UndoOk();
    void ThinkOk(QString msg);


    //-----------------FUNKCJE SZACHOWE-----------------//
    void TcpMoveOk();
    void resetBoardCompleted();

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    void setAI(bool bAI)                { m_bAI = bAI; } //todo: wszystkie nazwy "AI" zamienić na "Igor"
    void setAIAsPlayer2(bool bIsAIAsP2) { m_bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2()               { return m_bIsIgorsAiSimulatedAsPlayer2; }
    bool getAI()                        { return m_bAI; }


public slots:
    //--------KOMUNIKACJA Z CHENARD--------//
    void checkMsgForChenard(QString msg);
    void checkMsgFromChenard(QString tcpMsgType, QString tcpRespond);
    void EnemyStart(); //ogólna funkcja sterująca
};

#endif // IGORBOT_H

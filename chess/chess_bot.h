#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#pragma once

#include "arduinousb.h"
#include "chessboard.h"
#include "vars/basic_vars.h"

class ChessBot
{
    Q_OBJECT

private:
    ArduinoUsb *_pArduinoUsb; //reszta wskaznikow dziedziczona

    bool m_bAI;
    bool m_bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    bool m_bIsIgorsAiSimulatedAsPlayer2;
    QString _QStrAIPiecieFromTo; //zapamiętany kolejny ruch bota czekający na wywołanie

    void wrongTcpAnswer(QString msgType, QString respond);
    void checkAI();

public:
    ChessBot(Chessboard *pChessboard, ArduinoUsb *pArduinoUsb);
    ~ChessBot() {}

    //----------KOMUNIKACJA Z CHENARD----------//
    void UndoOk(); //todo: i co dalej to robi?
    void ThinkOk(QString msg); //todo: i co dalej to robi?

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    void setAI(bool bAI)                { m_bAI = bAI; } //todo: wszystkie nazwy "AI" zamienić na "Igor"
    void setAIAsPlayer2(bool bIsAIAsP2) { m_bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2() { return m_bIsIgorsAiSimulatedAsPlayer2; } //todo: is...
    bool getAI() { return m_bAI; } //todo: is...

public slots:
    void EnemyStart(); //ogólna funkcja sterująca (edit: todo: czym/jak/poco?)

};

#endif // CHESS_BOT_H

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

    bool _bAI;
    bool _bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    bool _bIsIgorsAiSimulatedAsPlayer2;
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
    void setAI(bool bAI)                { _bAI = bAI; } //todo: wszystkie nazwy "AI" zamienić na "Igor"
    void setAIAsPlayer2(bool bIsAIAsP2) { _bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2() { return _bIsIgorsAiSimulatedAsPlayer2; } //todo: is...
    bool getAI() { return _bAI; } //todo: is...

public slots:
    void EnemyStart(); //ogólna funkcja sterująca (edit: todo: czym/jak/poco?)

};

#endif // CHESS_BOT_H

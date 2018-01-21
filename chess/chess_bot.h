#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#pragma once

#include "chess.h"

class ChessBot
{
    Q_OBJECT

private:
    Chess* _pChess;

    bool _bAI;
    bool _bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    bool _bIsIgorsAiSimulatedAsPlayer2;
    QString _QStrAIPiecieFromTo; //zapamiętany kolejny ruch bota czekający na wywołanie

    void wrongTcpAnswer(QString msgType, QString respond);
    void checkAI();

public:
    ChessBot(Chess* pChess);
    ~ChessBot() {}

    //----------KOMUNIKACJA Z CHENARD----------//
    void UndoOk(); //future: i co dalej to robi?
    void ThinkOk(QString msg); //future: i co dalej to robi?

    //-----METODY-DOSTĘPOWE-DO-PÓL-----//
    void setAI(bool bAI)                { _bAI = bAI; } //future: wszystkie nazwy "AI" zamienić na "Igor"
    void setAIAsPlayer2(bool bIsAIAsP2) { _bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2() { return _bIsIgorsAiSimulatedAsPlayer2; } //future: is...
    bool getAI() { return _bAI; } //future: is...

public slots:
    void enemyStart(); //ogólna funkcja sterująca (edit: future: czym/jak/poco? - nazwa funckji)

};

#endif // CHESS_BOT_H

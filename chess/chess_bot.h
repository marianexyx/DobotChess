#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#pragma once
#include "chess.h"

class Chess;

class ChessBot
{
    friend class Chess;

private:
    Chess* _pChess;

    bool _bAI;
    bool _bUndo; //dzięki tej fladze będziemy wiedzieli czy odpowiedź...
    //...na 'status' z tcp dotyczy wykonanego ruchu gracza lub igora
    bool _bIsIgorsAiSimulatedAsPlayer2;
    QString _QStrAIPiecieFromTo; //zapamiętany kolejny ruch bota czekający na wywołanie

public:
    ChessBot(Chess* pChess);
    ~ChessBot() {}

    void undoOk(); //future: i co dalej to robi?
    void thinkOk(QString QStrMsg); //future: i co dalej to robi?
    void checkAI();

    void setAI(bool bAI) { _bAI = bAI; } //future: najgorsza nazwa
    void setAIAsPlayer2(bool bIsAIAsP2) { _bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2() { return _bIsIgorsAiSimulatedAsPlayer2; } //future: is...
    bool getAI() { return _bAI; } //future: is...

public slots:
    void enemyStart(); //ogólna funkcja sterująca (edit: future: czym/jak/poco? - nazwa funckji)
};

#endif // CHESS_BOT_H

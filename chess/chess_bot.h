#ifndef CHESS_BOT_H
#define CHESS_BOT_H

#pragma once
#include <QObject>
#include <QString>
#include <QDebug>
#include "vars/log.h"

class ChessBot: public QObject
{
    Q_OBJECT

    friend class Chess;

private:
    bool _bAI;
    bool _bUndo; //this flag determine is tcp answer for "status"...
    //...is from player or bot request
    bool _bIsIgorsAiSimulatedAsPlayer2;
    QString _QStrAIPiecieFromTo; //remebered next bot move to execute

public:
    ChessBot();
    ~ChessBot() {}

    //future: almost all of this functions have not best names:
    void undoOk();
    void thinkOk(QString QStrMsg);
    void checkAI();

    void setAI(bool bAI) { _bAI = bAI; }
    void setAIAsPlayer2(bool bIsAIAsP2) { _bIsIgorsAiSimulatedAsPlayer2 = bIsAIAsP2; }

    bool getAIAsPlayer2() { return _bIsIgorsAiSimulatedAsPlayer2; }
    bool getAI() { return _bAI; }

public slots:
    void enemyStart();
};

#endif // CHESS_BOT_H

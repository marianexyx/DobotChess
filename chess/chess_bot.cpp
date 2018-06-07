#include "chess_bot.h"

ChessBot::ChessBot()
{
    _bAI = false;
    _bUndo = false;
    _bIsIgorsAiSimulatedAsPlayer2 = false;
}

void ChessBot::undoOk()
{
    _bUndo = true; //safe that undo move has been made. let whole igor move execute, including...
    //...checking all types of moves, so it can be properly executed mechanicly
    //future: _pChess->sendDataToClient("move " + _QStrAIPiecieFromTo);
}

void ChessBot::thinkOk(QString QStrMsg)
{
    //future: emit _pChess->addTextToLogPTE("AI is ready to start move\n", LOG_CORE);

    _QStrAIPiecieFromTo = QStrMsg.mid(3,4); //safe made up bot move in memory
    //future: _pChess->sendMsgToTcp("undo 1"); //...and get back to state before bot move, so...
    //...core can check what kind of mechanical move it may be
}

void ChessBot::checkAI()
{
    if (!_bAI)
        qDebug() << "ERROR: ChessBot::checkAI(): initiated IgorBot method with AI turned off";
}

void ChessBot::enemyStart()
{
    this->checkAI();
    //furture: will this work, after code changes? not needed now
    //_pChess->resetPiecePositions();
}

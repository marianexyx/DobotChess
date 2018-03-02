#include "chess_bot.h"

ChessBot::ChessBot()
{
    _bAI = false;
    _bUndo = false;
    _bIsIgorsAiSimulatedAsPlayer2 = false;
}

void ChessBot::undoOk()
{
    _bUndo = true; //zapamiętaj że cofnięcie ruchu miało miejsce
    //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich...
    //...dziwnych ruchów tak aby też wykonał się dobrze mechanicznie
    //future: _pChess->sendDataToClient("move " + _QStrAIPiecieFromTo);
}

void ChessBot::thinkOk(QString QStrMsg)
{
    //future: emit _pChess->addTextToLogPTE("AI is ready to start move\n", LOG_CORE);

    _QStrAIPiecieFromTo = QStrMsg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    //future: _pChess->sendMsgToTcp("undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz...
    //...przeciągnąć go przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać...
    //...w szczególnych przypadkach.
}

void ChessBot::checkAI()
{
    if (!_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
        qDebug() << "ERROR: ChessBot::checkAI(): initiated IgorBot method with AI turned off";
}

void ChessBot::enemyStart()
{
    this->checkAI();
    //furture: a jak będzie z nowym resetem dla bota? olać póki nie potrzebuję
    //_pResets->resetPiecePositions();
}

#include "chess_bot.h"

ChessBot::ChessBot(Chess *pChess, ArduinoUsb *pArduinoUsb)
{
    _pChess = pChess;
    _pArduinoUsb = pArduinoUsb;

    _bAI = false;
    _bUndo = false;
    _bIsIgorsAiSimulatedAsPlayer2 = false;
}

void ChessBot::UndoOk()
{
    _bUndo = true; //zapamiętaj że cofnięcie ruchu miało miejsce
    //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich...
    //...dziwnych ruchów tak aby też wykonał się dobrze mechanicznie
    _pChess->sendDataToClient("move " + _QStrAIPiecieFromTo);
}

void ChessBot::ThinkOk(QString msg)
{
    emit _pChess->addTextToConsole("AI is ready to start move\n", LOG_CORE);
    qDebug() << "AI is ready to start move";

    _QStrAIPiecieFromTo = msg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    _pChess->sendMsgToTcp("undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz...
    //...przeciągnąć go przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać...
    //...w szczególnych przypadkach.
}

void ChessBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //future: zapanować jakoś nad tymi sygnałami konsoli
    emit _pChess->addTextToConsole("ERROR: ChessBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                respond + "for tcpMsgType = " + msgType + "\n", LOG_CORE);
    qDebug() << "ERROR: ChessBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void ChessBot::checkAI()
{
    if (!_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        emit _pChess->addTextToConsole("ERROR: ChessBot::checkAI(): initiated IgorBot method "
                                       "with AI turned off\n", LOG_NOTHING);
        qDebug() << "ERROR: ChessBot::checkAI(): initiated IgorBot method with AI turned off";
    }
}

void ChessBot::enemyStart()
{
    this->checkAI();
    //furture: a jak będzie z nowym resetem dla bota? olać póki nie potrzebuję
    _pChess->getResetsPointer()->resetPiecePositions();
}

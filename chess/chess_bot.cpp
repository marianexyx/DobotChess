#include "chess_bot.h"

ChessBot::ChessBot(Chessboard *pChessboard, ArduinoUsb *pArduinoUsb)
{
    _pChessboard = pChessboard;
    _pArduinoUsb = pArduinoUsb;

    m_bAI = false;
    m_bUndo = false;
    m_bIsIgorsAiSimulatedAsPlayer2 = false;
}

void ChessBot::UndoOk()
{
    m_bUndo = true; //zapamiętaj że cofnięcie ruchu miało miejsce
    //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich...
    //...dziwnych ruchów tak aby też wykonał się dobrze mechanicznie
    this->checkMsgForChenard("move " + _QStrAIPiecieFromTo);
}

void ChessBot::ThinkOk(QString msg)
{
    emit this->addTextToConsole("AI is ready to start move\n", LOG_CORE);
    qDebug() << "AI is ready to start move";

    _QStrAIPiecieFromTo = msg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    this->SendMsgToTcp("undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz przeciągnąć go...
    //...przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać w szczególnych przypadkach.
}

void ChessBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //TODO: zapanować jakoś nad tymi sygnałami konsoli
    emit this->addTextToConsole("ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                respond + "for tcpMsgType = " + msgType + "\n", LOG_CORE);
    qDebug() << "ERROR: IgorBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void ChessBot::checkAI()
{
    if (!m_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        emit this->addTextToConsole("ERROR: initiated IgorBot method with AI turned off\n", LOG_NOTHING);
        qDebug() << "ERROR: initiated IgorBot method with AI turned off";
    }
}

void ChessBot::EnemyStart()
{
    this->checkAI();
    this->resetPiecePositions();
}

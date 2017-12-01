#include "igorbot.h"

IgorBot::IgorBot(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
                 ArduinoUsb *pArduinoUsb)
{
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pTCPMsgs = pTCPMsgs;
    _pArduinoUsb = pArduinoUsb;

    m_bAI = false;
    m_bUndo = false;
    m_bIsIgorsAiSimulatedAsPlayer2 = false;
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z ARDUINO------------------------------//
//-------------------------------------------------------------------------------//
void IgorBot::GameInProgress()
{
    if (!m_bUndo) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
    {
        if (m_bIsIgorsAiSimulatedAsPlayer2)
            this->SendDataToPlayer("EnterSimulatedIgorsMove");
        else this->Think5000(); //wymyśl kolejny ruch bota białego Igora
    }
    else  //a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku
    {
        m_bUndo = false;
        this->SendDataToPlayer("IgorHasEndedMove"); //niech gracz wykonuje swój kolejny ruch
    }
}

void IgorBot::SendDataToPlayer(QString msg)
{
    //todo: nieujednolicone typy komunikatów web z arduino są, przez co...
    //...takie dziwne zmiany w locie (wynika to z virtualow i dziedziwczenia)
    if (msg.contains("promote")) msg = "promote";
    else if (msg.contains("newOk")) msg = "started";
    else if (msg.contains("badMove")) msg = "BAD_MOVE";

    qDebug() << "Sending to arduino:" << msg;
    this->addTextToConsole("Sending to arduino: " + msg, LOG_CORE);

    _pArduinoUsb->sendDataToUsb(msg);
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z CHENARD------------------------------//
//-------------------------------------------------------------------------------//

void IgorBot::EnemyStart()
{
    this->checkAI();
    this->resetPiecePositions();
}

void IgorBot::checkMsgForChenard(QString msg)
{
    qDebug() << "IgorBot::checkMsgForChenard: received: " << msg;
    if (msg == "new") this->NewGame();
    else if (msg.left(4) == "move") this->handleMove(msg.mid(5));
    else if (msg.left(9) == "promoteTo") this->Promote(msg.mid(10));
    else if (msg.left(5) == "reset") this->resetPiecePositions();
    else qDebug() << "ERROR: received not recognized msg in IgorBot::checkMsgForChenard: " << msg;
}

void IgorBot::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    this->checkAI();

    if (tcpMsgType == "new" && (tcpRespond == "OK\n" || tcpRespond == "OK"))
    {
        this->Status();
        this->GameStarted();
    }
    else if (tcpMsgType.left(4) == "move")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego 2 warunki
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") this->TcpMoveOk();
        else if (tcpRespond.left(8) == "BAD_MOVE") this->BadMove(tcpRespond);
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        _pChessboard->saveStatusData(tcpRespond);
        if (_pChessboard->getGameStatus() == "*")
        {
            this->AskForHistoryMoves();
        }
        else
        {
            _pChessboard->clearLegalMoves();
            _pChessboard->clearHistoryMoves();
            this->EndOfGame(tcpRespond);
        }
    }
    else if (tcpMsgType == "undo 1" && (tcpRespond == "OK\n" || tcpRespond == "OK"))
    {
        this->UndoOk();
    }
    else if (tcpMsgType == "think 5000" && (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1"))
    {
        this->ThinkOk(tcpRespond); //"f.e.: OK d1h5 Qh5#"
    }
    else if (tcpMsgType == "legal" && (tcpRespond.left(3) == "OK "))
    {
        this->legalOk(tcpRespond);
    }
    else wrongTcpAnswer(tcpMsgType, tcpRespond);
}

void IgorBot::NewGame()
{
    qDebug() << "Sending to tcp: new";
    emit this->addTextToConsole("Sending to tcp: new\n", LOG_CORE);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "new");
}

void IgorBot::MoveTcpPiece(QString msg)
{
    qDebug() << "IgorBot::MoveTcpPiece: Sending move to tcp: " << msg;
    emit this->addTextToConsole("Sending move to tcp: " + msg + "\n", LOG_CORE);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, msg);
}

void IgorBot::Status()
{
    this->addTextToConsole("Sending to tcp: status\n", LOG_CORE);
    qDebug() << "Sending to tcp: status";
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "status");
}

void IgorBot::Promote(QString msg)
{
     this->listMovesForDobot(ST_PROMOTION);
    _pChessboard->setMoveType(ST_PROMOTION);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "move " + _pChessboard->QStrFuturePromote + msg);
    _pChessboard->QStrFuturePromote.clear();
}

void IgorBot::AskForLegalMoves()
{
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "legal");
}

void IgorBot::AskForHistoryMoves()
{
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "history pgn");
}

void IgorBot::Think5000()
{
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "think 5000");
}

void IgorBot::UndoOk()
{
    m_bUndo = true; //zapamiętaj że cofnięcie ruchu miało miejsce
    //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich...
    //...dziwnych ruchów tak aby też wykonał się dobrze mechanicznie
    this->checkMsgForChenard("move " + _pChessboard->QStrAIPiecieFromTo);
}

void IgorBot::ThinkOk(QString msg)
{
    emit this->addTextToConsole("AI is ready to start move\n", LOG_CORE);
    qDebug() << "AI is ready to start move";

    _pChessboard->QStrAIPiecieFromTo = msg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz przeciągnąć go...
    //...przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać w szczególnych przypadkach.
}

void IgorBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //TODO: zapanować jakoś nad tymi sygnałami konsoli
    emit this->addTextToConsole("ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                respond + "for tcpMsgType = " + msgType + "\n", LOG_CORE);
    qDebug() << "ERROR: IgorBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void IgorBot::checkAI()
{
    if (!m_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        emit this->addTextToConsole("ERROR: initiated IgorBot method with AI turned off\n", LOG_NOTHING);
        qDebug() << "ERROR: initiated IgorBot method with AI turned off";
    }
}

//-----------------FUNKCJE SZACHOWE-----------------//
void IgorBot::TcpMoveOk()
{
    qDebug() << "IgorBot::TcpMoveOk()";

    this->GameInProgress();
    this->Status();
}

void IgorBot::resetBoardCompleted()
{
    this->NewGame();
}


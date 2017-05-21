#include "igorbot.h"

IgorBot::IgorBot(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
                 WebTable *pWebTable, ArduinoUsb *pArduinoUsb)
{
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pTCPMsgs = pTCPMsgs;
    _pWebTable = pWebTable;
    _pArduinoUsb = pArduinoUsb;

    m_bAI = false;
    m_bUndo = false;
    m_bIsIgorsAiSimulatedAsPlayer2 = false;
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z ARDUINO------------------------------//
//-------------------------------------------------------------------------------//

void IgorBot::GameStarted()
{
    emit this->addTextToConsole("new_game\n", USB_SENT);
    qDebug() << "Sending to USB: new_game";
    _pArduinoUsb->sendDataToUsb("started"); //na arduino daj możliwość już wciśnięcua start
}

void IgorBot::BadMove(QString msg)
{
    qDebug() << "Bad move:" << msg << ". Sending to USB: BAD_MOVE";
    emit this->addTextToConsole("Bad move: " + msg + ". Sending to USB: BAD_MOVE\n", CORE);

    //todo: simplified() nie usuwa tylko podmienia białe znaki. sprawdzić to wszędzie
    //_pArduinoUsb->sendDataToUsb(msg.simplified());
    _pArduinoUsb->sendDataToUsb("BAD_MOVE");
}

void IgorBot::GameInProgress()
{
    //podaj na stronę info o tym że ruch został wykonany
    qDebug() << "IgorBot::GameInProgress(): Sending to Arduino: game_in_progress";
    emit this->addTextToConsole("game_in_progress", CORE);

    if (!m_bUndo) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku
    {
        if (m_bIsIgorsAiSimulatedAsPlayer2) _pArduinoUsb->sendDataToUsb("EnterSimulatedIgorsMove");
        else this->Think5000(); //wymyśl kolejny ruch bota białego Igora
    }
    else  //a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku
    {
        m_bUndo = false;
        _pArduinoUsb->sendDataToUsb("IgorHasEndedMove"); //niech gracz wykonuje swój kolejny ruch
    }
}

void IgorBot::EndOfGame(QString msg)
{
    QString QsWhoWon;
    if (msg.left(3) == "1-0") QsWhoWon = "white_won";
    else if (msg.left(3) == "0-1") QsWhoWon = "black_won";
    else if (msg.left(7) == "1/2-1/2") QsWhoWon = "draw";

    qDebug() << "Sending to usb: " << QsWhoWon;
    _pArduinoUsb->sendDataToUsb("GameOver: " + QsWhoWon);

    this->resetPiecePositions(); //przywróć wszystkie bierki do stanu początkowego
    //TODO: zablokować możliwość robienia czegokolwiek na stronie/arduino aż to się nie zakończy
}

void IgorBot::PromoteToWhat(QString moveForFuturePromote)
{
    _pChessboard->QStrFuturePromote = moveForFuturePromote;
    qDebug() << "Sending to arduino: promote";
    this->addTextToConsole("Sending to arduino: promote", CORE);
    _pArduinoUsb->sendDataToUsb("promote"); //zapytaj się arduino na co ma być ta promocja
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
            this->AskForLegalMoves();
        }
        else
        {
            _pChessboard->clearLegalMoves();
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
    emit this->addTextToConsole("Sending to tcp: new\n", CORE);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "new");
}

void IgorBot::MoveTcpPiece(QString msg)
{
    qDebug() << "IgorBot::MoveTcpPiece: Sending move to tcp: " << msg;
    emit this->addTextToConsole("Sending move to tcp: " + msg + "\n", CORE);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, msg);
}

void IgorBot::Status()
{
    this->addTextToConsole("Sending to tcp: status\n", CORE);
    qDebug() << "Sending to tcp: status";
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "status");
}

void IgorBot::Promote(QString msg)
{
     this->pieceMovingSequence(PROMOTION);
    _pChessboard->setMoveType(PROMOTION);
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "move " + _pChessboard->QStrFuturePromote + msg);
    _pChessboard->QStrFuturePromote.clear();
}

void IgorBot::AskForLegalMoves()
{
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "legal");
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
    this->checkMsgForChenard("move " + _pChessboard->QsAIPiecieFromTo);
}

void IgorBot::ThinkOk(QString msg)
{
    emit this->addTextToConsole("AI is ready to start move\n", CORE);
    qDebug() << "AI is ready to start move";

    _pChessboard->QsAIPiecieFromTo = msg.mid(3,4); //zapisz w pamięci ruch wymyślony przez bota
    _pTCPMsgs->TcpQueueMsg(ARDUINO, "undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz przeciągnąć go...
    //...przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać w szczególnych przypadkach.
}

void IgorBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //TODO: zapanować jakoś nad tymi sygnałami konsoli
    emit this->addTextToConsole("ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                respond + "for tcpMsgType = " + msgType + "\n", CORE);
    qDebug() << "ERROR: IgorBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void IgorBot::checkAI()
{
    if (!m_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        emit this->addTextToConsole("ERROR: initiated IgorBot method with AI turned off\n", NOTHING);
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


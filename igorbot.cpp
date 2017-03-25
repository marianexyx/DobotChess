#include "igorbot.h"

IgorBot::IgorBot(Chessboard *pChessboard, TCPMsgs *pTCPMsgs, ArduinoUsb *pArduinoUsb, Chess *pChes)
{
    _pChessboard = pChessboard;
    _pTCPMsgs = pTCPMsgs;
    _pArduinoUsb = pArduinoUsb;
    _pChess = pChess;

    m_bAI = false;
    m_bUndo = false;
}

void IgorBot::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    this->checkAI();

    if (tcpMsgType == "new")
    {
        if (tcpRespond == "OK\n" || tcpRespond == "OK")
            _pArduinoUsb->sendDataToUsb("started"); //na arduino daj możliwość już wciśnięcua start
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType.left(4) == "move")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego 2 warunki
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") _pChess->MoveOk(ARDUINO);
        else if (tcpRespond == "BAD_MOVE")
        {
            emit _pChess->addTextToCoreConsole("BAD_MOVE request from Arduino\n", 'c');
            qDebug() << "BAD_MOVE request from Arduino";

            _pArduinoUsb->sendDataToUsb(QsTcpRespond); //np. "BAD_MOVE e2e4"
        }
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType.left(4) == "test") //dla zapytania: 'test e2e4' dostaniemy: 'OK e2e4 (...)'.
    {
        if (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1") _pChess->TestOk();
        else if (tcpRespond == "ILLEGAL") _pChess->MovePiece(ARDUINO, _pChessboard->QsPiecieFromTo);
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        if (tcpRespond.left(1) == "*")
        {
            if (!m_bUndo) //jeżeli po wykonaniu ruchu gracza gra jest dalej w toku...
                _pTCPMsgs->queueMsgs(ARDUINO, "think 5000"); //...to wymyśl kolejny ruch bota białego Igora...
            else  //...a jeżeli po wykonaniu ruchu Igora gra jest dalej w toku...
            {
                m_bUndo = false;
                _pArduinoUsb->sendDataToUsb("IgorHasEndedMove"); //...to niech gracz wykonuje swój kolejny ruch.
            }
        }
        else  if (tcpRespond.left(3) == "1-0" || tcpRespond.left(3) == "0-1" || tcpRespond.left(7) == "1/2-1/2")
        {
            QString QsWhoWon;
            if (tcpRespond.left(3) == "1-0") QsWhoWon = "white_won";
            else if (tcpRespond.left(3) == "0-1") QsWhoWon = "black_won";
            else if (tcpRespond.left(7) == "1/2-1/2") QsWhoWon = "draw";

            _pArduinoUsb->sendDataToUsb("GameOver: " + QsWhoWon);
        }
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "undo 1")
    {
        if (tcpRespond == "OK\n")
        {
            m_bUndo = true;
            //niech się wykona cały ruch Igora, łącznie ze sprawdzeniem wszystkich dziwnych ruchów...
            //...tak aby też wykonał się dobrze mechanicznie
            _pChess->checkMsgFromWebsockets("move " + _pChessboard->QsAIPiecieFromTo);
        }
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "think 5000") //jeżeli mamy doczynienia z botem, który wymyślił ruch ...
    {
        if (tcpRespond.left(3) == "OK " && tcpRespond.left(4) != "OK 1") //"OK d1h5 Qh5#"
        {
            _pChessboard->QsAIPiecieFromTo = tcpRespond.mid(3,4); //...to zapisz ten ruch w pamięci...
            _pTCPMsgs->queueMsgs(ARDUINO, "undo 1"); //...i wróć do stanu sprzed ruchu Igora, by zaraz przeciągnąć go...
            //...przez cały kod sprawdzający ruchy, by wiedzieć jak ramie ma się poruszać w szczególnych przypadkach.

            emit _pChess->addTextToCoreConsole("AI is ready to start move\n", 'c');
            qDebug() << "AI is ready to start move";
        }
        else wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
}

void IgorBot::wrongTcpAnswer(QString msgType, QString respond)
{
    this->checkAI();

    //TODO: zapanować jakoś nad tymi sygnałami konsoli
     emit _pChess->addTextToCoreConsole("ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " +
                                  respond + "for tcpMsgType = " + msgType + "\n", 'c');
    qDebug() << "ERROR: IgorBot::checkMsgFromChenard(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void IgorBot::checkAI()
{
    if (!m_bAI) //wywal błąd jeżeli próbuje wywołać metodę w tej klasie bez włączonej SI
    {
        _pChess->addTextToCoreConsole("ERROR: initiated IgorBot method with AI turned off\n", '0');
        qDebug() << "ERROR: initiated IgorBot method with AI turned off";
    }
}

void IgorBot::enemyStart()
{
    this->checkAI();

    _pChess->resetPiecePositions(); //przywróć bierki na pierwotne pozycje
    _pChess->NewGame(); //zacznij w pamięci chenardu nową grę
}

QString IgorBot::enemySend(QString QsFT)
{
    this->checkAI();

    QString QsRespond;
    if (QsFT.length() == 4) //np. 'e2e4'
    {
        //TODO: nie ogarniam wyrażeń regularnych:
        if ((QsFT.at(0) == 'a' || QsFT.at(0) == 'b' || QsFT.at(0) == 'c' || QsFT.at(0) == 'd' ||
             QsFT.at(0) == 'e' || QsFT.at(0) == 'f' || QsFT.at(0) == 'g' || QsFT.at(0) == 'h') &&
                (QsFT.at(1) == '1' || QsFT.at(1) == '2' || QsFT.at(1) == '3' || QsFT.at(1) == '4' ||
                 QsFT.at(1) == '5' || QsFT.at(1) == '6' || QsFT.at(1) == '7' || QsFT.at(1) == '8') &&
                (QsFT.at(2) == 'a' || QsFT.at(2) == 'b' || QsFT.at(2) == 'c' || QsFT.at(2) == 'd' ||
                 QsFT.at(2) == 'e' || QsFT.at(2) == 'f' || QsFT.at(2) == 'g' || QsFT.at(2) == 'h') &&
                (QsFT.at(3) == '1' || QsFT.at(3) == '2' || QsFT.at(3) == '3' || QsFT.at(3) == '4' ||
                 QsFT.at(3) == '5' || QsFT.at(3) == '6' || QsFT.at(3) == '7' || QsFT.at(3) == '8'))
        {
            QsRespond = "move " + QsFT;
            _pWebsockets->sendToChess(QsRespond); //wyślij zapytanie o ruch tak, jakby...
            //...szło ono ze strony
        }
        else QsRespond = "ERROR: Chess::AIEnemySend(: Wrong square positions\n";
    }
    else QsRespond = "ERROR: Chess::AIEnemySend(: Wrong lenght of move cmd\n";

    return QsRespond;
}

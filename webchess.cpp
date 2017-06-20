#include "webchess.h"

WebChess::WebChess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
                   WebTable *pWebTable, Websockets *pWebsockets)
{
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pTCPMsgs = pTCPMsgs;
    _pWebTable = pWebTable;
    _pWebsockets = pWebsockets;
}

//---------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z WWW------------------------------//
//---------------------------------------------------------------------------//

void WebChess::GameStarted()
{
    _pWebsockets->addTextToConsole("newGame\n", WEBSOCKET);
    qDebug() << "Sending 'newOk' to site";

    if (!_bServiceTests)_pWebsockets->sendMsg("newOk");
}

void WebChess::BadMove(QString msg)
{
    qDebug() << "Bad move:" << msg << ". Sending to WS: badMove";
    emit this->addTextToConsole("Bad move: " + msg + "\n", CORE);

    _pWebsockets->sendMsg("badMove " + msg);
}

void WebChess::GameInProgress()
{
    qDebug() << "Chess::GameInProgress(): Sending to WS: moveOk" <<
                _pChessboard->getPiecieFromTo() <<
                _pChessboard->getStrWhoseTurn() << "continue";

    _pWebsockets->sendMsg("moveOk " + _pChessboard->getPiecieFromTo() + " " +
                          _pChessboard->getStrWhoseTurn() + " continue");
}

void WebChess::EndOfGame(QString msg)
{
    QString whoWon;
    if (msg.left(3) == "1-0") whoWon = "whiteWon";
    else if (msg.left(3) == "0-1") whoWon = "blackWon";
    else if (msg.left(7) == "1/2-1/2") whoWon = "draw";

    qDebug() << "Sending to WS: moveOk " << _pChessboard->getPiecieFromTo() << " nt " << whoWon;

    _pWebsockets->sendMsg("moveOk " + _pChessboard->getPiecieFromTo() + " nt " + whoWon);

    this->resetPiecePositions();
}

void WebChess::PromoteToWhat(QString moveForFuturePromote)
{
    _pChessboard->QStrFuturePromote = moveForFuturePromote;

    qDebug() << "Sending to WS: moveOk" << moveForFuturePromote <<
                _pChessboard->getStrWhoseTurn() << "promote";


    _pWebsockets->sendMsg("moveOk " + moveForFuturePromote + " " +
                                      _pChessboard->getStrWhoseTurn() + " promote");
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z CHENARD------------------------------//
//-------------------------------------------------------------------------------//

void WebChess::checkMsgForChenard(QString msgFromWs)
{
    qDebug() << "WebChess::checkMsgForChenard: received: " << msgFromWs;
    if (msgFromWs == "newGame") this->NewGame();
    else if (msgFromWs.left(4) == "move") this->handleMove(msgFromWs.mid(5));
    else if (msgFromWs.left(9) == "promoteTo") this->Promote(msgFromWs.right(1));
    else if (msgFromWs.left(5) == "reset") this->resetPiecePositions();
    else qDebug() << "ERROR: received not recognized msg in WebChess::checkMsgForChenard: " << msgFromWs;
}

void WebChess::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    qDebug() << "WebChess::checkMsgFromChenard: tcpMsgType=" << tcpMsgType <<
                ", tcpRespond:" << tcpRespond;

    if (tcpMsgType == "new")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki
        if (tcpRespond == "OK\n" || tcpRespond == "OK")
        {
            this->Status();
            this->GameStarted();
        }
        else
            this->wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType.left(4) == "move")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki poniżej
        if (tcpRespond == "OK 1\n" || tcpRespond == "OK 1") this->TcpMoveOk();
        else if (tcpRespond/*.left(8)*/ == "BAD_MOVE") this->BadMove(tcpRespond);
        else this->wrongTcpAnswer(tcpMsgType, tcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        _pChessboard->saveStatusData(tcpRespond);

        if (_pChessboard->getGameStatus().left(1) == "*")
        {
            this->AskForLegalMoves();
        }
        else if (_pChessboard->getGameStatus().left(3) == "1-0" ||
                 _pChessboard->getGameStatus().left(3) == "0-1" ||
                 _pChessboard->getGameStatus().left(7) == "1/2-1/2")
        {
            _pChessboard->clearLegalMoves();
            this->EndOfGame(tcpRespond);
        }
        else
            this->wrongTcpAnswer(tcpMsgType, _pChessboard->getGameStatus());
    }
    else if (tcpMsgType == "legal" && (tcpRespond.left(3) == "OK "))
    {
        this->legalOk(tcpRespond);
    }
    else qDebug() << "ERROR: WebChess:checkMsgFromChenard() received unknown tcpMsgType: " << tcpMsgType;
}

void WebChess::Promote(QString msg)
{
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "move " + _pChessboard->QStrFuturePromote + msg);
    _pChessboard->QStrFuturePromote.clear();
}

void WebChess::NewGame() //przesyłanie prośby o nową grę na TCP
{
    if ((_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
         !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) || //jeżeli obaj gracze siedzą przy stole
            _bServiceTests) //albo mamy do czynienia z zapytaniem serwisowym
        //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
    {
        _pWebsockets->addTextToConsole("Sending 'new' game command to tcp \n", WEBSOCKET);
        _pTCPMsgs->TcpQueueMsg(WEBSITE, "new");
    }
    else _pWebsockets->addTextToConsole("ERROR: Chess::NewGame(): Wrong players names\n", WEBSOCKET);
}

void WebChess::MoveTcpPiece(QString msg) // żądanie ruchu- przemieszczenie bierki.
{
    //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    _pWebsockets->addTextToConsole("WebChess::MoveTcpPiece: Sending normal move to tcp: " + msg + "\n", WEBSOCKET);
    qDebug() << "WebChess::MoveTcpPiece: Sending normal move to tcp: " << msg;
    _pTCPMsgs->TcpQueueMsg(WEBSITE, msg); //zapytaj się tcp o poprawność prośby o ruch
}

void WebChess::Status()
{
    _pWebsockets->addTextToConsole("Sending to tcp: status\n", CORE);
    qDebug() << "Sending to tcp: status";
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "status");
}

void WebChess::AskForLegalMoves()
{
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "legal");
}

//-----------------FUNKCJE SZACHOWE-----------------//
void WebChess::TcpMoveOk()
{
    qDebug() << "WebChess::TcpMoveOk()";

    this->GameInProgress();
    this->Status();
}

void WebChess::resetBoardCompleted()
{
    this->NewGame();
}

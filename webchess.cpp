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

    if (!_bServiceTests)
    {
        _pChessboard->resetTimers();
        _pChessboard->startGameTimer();
        _pWebsockets->sendMsg("newOk");
    }
}

void WebChess::BadMove(QString msg)
{
    qDebug() << "Bad move:" << msg << ". Sending to WS: badMove";
    emit this->addTextToConsole("Bad move: " + msg + "\n", CORE);

    _pWebsockets->sendMsg("badMove " + msg + " " + _pChessboard->getStrWhoseTurn());
}

void WebChess::GameInProgress()
{
    qDebug() << "Chess::GameInProgress(): Sending to WS: moveOk" <<
                _pChessboard->getPiecieFromTo() <<
                _pChessboard->getStrWhoseTurn() << "continue";

    _pChessboard->switchPlayersTimers();
    _pWebsockets->sendMsg("moveOk " + _pChessboard->getSiteMoveRequest() + " " +
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
    //todo: wygląda na to że funkcja resetu załącza się jeszcze zanim odpowiedź poleci na stronę,
    //przez co trzeba czekać aż resetowanie się zakończy zanim gracze się dowiedzą że nastąpił koniec gry
    //todo: stworzyć funkcję czyszczącą masę rzeczy przy różnych warunkach jak koniec gry
    _pChessboard->resetTimers();
    this->reset();
}

void WebChess::PromoteToWhat(QString moveForFuturePromote)
{
    _pChessboard->QStrFuturePromote = moveForFuturePromote;

    qDebug() << "Sending to WS: moveOk" << moveForFuturePromote <<
                _pChessboard->getStrWhoseTurn() << "promote";

    _pChessboard->switchPlayersTimers();
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
    else if (msgFromWs.left(5) == "reset") this->reset();
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
            _pChessboard->saveStatusData("* rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n");
            this->legalOk("OK 20 b1c3 b1a3 g1h3 g1f3 a2a3 a2a4 b2b3 b2b4 c2c3 c2c4 d2d3 d2d4 e2e3 e2e4 "
                          "f2f3 f2f4 g2g3 g2g4 h2h3 h2h4");
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
            this->GameInProgress(); //TODO: zrobić to kiedyś tak by dopiero w odpowiedzi na legal..
            //...wysyłał na stronę potwiedzenie wykonania ruchu (tj. zdjął blokadę przed kojenym ruchem)
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
    _pWebsockets->addTextToConsole("Sending normal move to tcp: " + msg + "\n", WEBSOCKET);
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

    this->Status();
}
void WebChess::reset()
{
    _pWebsockets->sendMsg("reseting");
    this->resetPiecePositions();
}

void WebChess::resetBoardCompleted()
{
    //TODO: prewencyjnie ustawić wszystkie wartości na startowe (rozpisać to: jakie, które i po co)
    _pWebsockets->sendMsg("ready");
}

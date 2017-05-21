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
    _pWebsockets->addTextToConsole("new_game\n", WEBSOCKET);
    qDebug() << "Sending 'new_game' to site";

    if (!_bServiceTests)_pWebsockets->processWebsocketMsg("new_game");
}

void WebChess::BadMove(QString msg)
{
    qDebug() << "BAD_MOVE:" + msg ;
    emit this->addTextToConsole("BAD_MOVE: " + msg + "\n", CORE);

    _pWebsockets->processWebsocketMsg("BAD_MOVE " + msg);
}

void WebChess::GameInProgress() //gra w toku
{
    //podaj na stronę info o tym że ruch został wykonany
    qDebug() << "Chess::GameInProgress(): Sending to Websockets: game_in_progress "
             << _pChessboard->QsPiecieFromTo;
    _pWebsockets->processWebsocketMsg("game_in_progress " + _pChessboard->QsPiecieFromTo);
}

void WebChess::EndOfGame(QString msg)
{
    QString QsWhoWon;
    if (msg.left(3) == "1-0") QsWhoWon = "white_won";
    else if (msg.left(3) == "0-1") QsWhoWon = "black_won";
    else if (msg.left(7) == "1/2-1/2") QsWhoWon = "draw";

    qDebug() << "Sending '" << QsWhoWon << "' to websockets";
    _pWebsockets->processWebsocketMsg(QsWhoWon);

    this->resetPiecePositions(); //przywróć wszystkie bierki do stanu początkowego
}

void WebChess::PromoteToWhat(QString moveForFuturePromote)
{
    _pChessboard->QStrFuturePromote = moveForFuturePromote;
    qDebug() << "Sending 'promote_to_what' to websockets";
    _pWebsockets->processWebsocketMsg("promote_to_what"); //to trzeba jeszcze zapytać się na WWW na co ma być ta promocja.
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z CHENARD------------------------------//
//-------------------------------------------------------------------------------//

void WebChess::checkMsgForChenard(QString msgFromWs)
{
    qDebug() << "WebChess::checkMsgForChenard: received: " << msgFromWs;
    if (msgFromWs == "new") this->NewGame();
    else if (msgFromWs.left(4) == "move") this->handleMove(msgFromWs.mid(5));
    else if (msgFromWs.left(10) == "promote_to") this->Promote(msgFromWs);
    else if (msgFromWs.left(5) == "reset") this->resetPiecePositions();
    else qDebug() << "ERROR: received not recognized msg in WebChess::checkMsgForChenard: " << msgFromWs;
}

void WebChess::checkMsgFromChenard(QString tcpMsgType, QString tcpRespond)
{
    qDebug() << "WebChess::checkMsgFromChenard: " << tcpRespond;

    if (tcpMsgType == "new")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki poniżej
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
        if (tcpRespond.left(1) == "*") this->GameInProgress(); //ruch wykonany poprawnie. gra w toku
        else if (tcpRespond.left(3) == "1-0" || tcpRespond.left(3) == "0-1" ||
                 tcpRespond.left(7) == "1/2-1/2") this->EndOfGame(tcpRespond);
        else this->wrongTcpAnswer(tcpMsgType, tcpRespond);

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

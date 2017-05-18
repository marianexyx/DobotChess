#include "webchess.h"

#define ROW 1
#define COLUMN 0

#define OPEN 1
#define CLOSE 0

#define FROM 1
#define TO 0

#define UP 1
#define DOWN 0

#define WEBSITE 1
#define ARDUINO 2

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

void WebChess::GameStarted() //zareaguj na to że gra wystartowała
{
    _pWebsockets->addTextToConsole("new_game\n", WEBSOCKET);
    qDebug() << "Sending 'new_game' to site";
    if (!_bServiceTests)_pWebsockets->processWebsocketMsg("new_game");
}

void WebChess::BadMove(QString msg)
{
    msg = msg.mid(9);
    qDebug() << "Sending 'BAD_MOVE...' to websockets";
    _pWebsockets->processWebsocketMsg("BAD_MOVE " + msg.simplified());
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

void WebChess::checkMsgFromChenard(QString tcpMsgType, QString QsTcpRespond)
{
    qDebug() << "WebChess::checkMsgFromChenard: " << QsTcpRespond;

    if (tcpMsgType == "new")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki poniżej
        if (QsTcpRespond == "OK\n" || QsTcpRespond == "OK")
            this->GameStarted();
        else
            this->wrongTcpAnswer(tcpMsgType, QsTcpRespond);
    }
    else if (tcpMsgType.left(4) == "move")
    {
        //zdarza się, że z jakiegoś powodu tcp utnie końcówkę '\n', dlatego są 2 warunki poniżej
        if (QsTcpRespond == "OK 1\n" || QsTcpRespond == "OK 1") this->TcpMoveOk();
        else if (QsTcpRespond/*.left(8)*/ == "BAD_MOVE") this->BadMove(QsTcpRespond);
        else this->wrongTcpAnswer(tcpMsgType, QsTcpRespond);
    }
    else if (tcpMsgType == "status")
    {
        if (QsTcpRespond.left(1) == "*") this->GameInProgress(); //ruch wykonany poprawnie. gra w toku
        else if (QsTcpRespond.left(3) == "1-0" || QsTcpRespond.left(3) == "0-1" ||
                 QsTcpRespond.left(7) == "1/2-1/2") this->EndOfGame(QsTcpRespond);
        else this->wrongTcpAnswer(tcpMsgType, QsTcpRespond);
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
{ //TODO: mylne jest wrażenie że ta funckja już wykonuje ruch bierką
    //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    _pWebsockets->addTextToConsole("WebChess::MoveTcpPiece: Sending normal move to tcp: " + msg + "\n", WEBSOCKET);
    qDebug() << "WebChess::MoveTcpPiece: Sending normal move to tcp: " << msg;
    _pTCPMsgs->TcpQueueMsg(WEBSITE, msg); //zapytaj się tcp o poprawność prośby o ruch
}

void WebChess::Status()
{
    _pWebsockets->addTextToConsole("Sending 'status' command to tcp. \n", WEBSOCKET);
    qDebug() << "Sending 'status' command to tcp";
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "status");
}

void WebChess::AskForLegalMoves()
{
    _pTCPMsgs->TcpQueueMsg(WEBSITE, "legal");
}

//-----------------FUNKCJE SZACHOWE-----------------//
void WebChess::TcpMoveOk()
{
    //todo
}

void WebChess::resetBoardCompleted()
{
    //todo
}

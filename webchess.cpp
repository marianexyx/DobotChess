#include "webchess.h"

#define ARDUINO 1
#define WEBSITE 2

WebChess::WebChess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
                   WebTable *pWebTable, Websockets *pWebsockets)
    :_nCommunicationType(WEBSITE)
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
    _pWebsockets->addTextToConsole("new_game\n", 'w');
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
    //TODO: zablokować możliwość robienia czegokolwiek na stronie/arduino aż to się nie zakończy
}

void WebChess::PromoteToWhat()
{
    qDebug() << "Sending 'promote_to_what' to websockets";
    _pWebsockets->processWebsocketMsg("promote_to_what"); //to trzeba jeszcze zapytać się na WWW na co ma być ta promocja.
}

//-------------------------------------------------------------------------------//
//----------------------------KOMUNIKACJA Z CHENARD------------------------------//
//-------------------------------------------------------------------------------//

void WebChess::checkMsgForChenard(QString msgFromWs)
{
    qDebug() << "Chess::checkMsgFromWebsockets: received: " << msgFromWs;
    if (msgFromWs == "new") this->NewGame();
    else if (msgFromWs.left(4) == "move") this->TestMove(msgFromWs); //sprawdź najpierw czy nie występują ruchy specjalne
    else if (msgFromWs.left(10) == "promote_to") this->Promote(msgFromWs); //odp. z WWW odnośnie tego na co promujemy
    else if (msgFromWs.left(5) == "reset") this->resetPiecePositions(); //przywróć bierki na szachownicę do stanu startowego
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgForChenard: " << msgFromWs;
}

void WebChess::checkMsgFromChenard(QString tcpMsgType, QString QsTcpRespond)
{
    //TODO: ruchy z MoveOk i TestOk wykonywać w jednej funkcji dla porządku kodu?
    qDebug() << "Chess::checkMsgFromChenard: " << QsTcpRespond;

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
        if (QsTcpRespond == "OK 1\n" || QsTcpRespond == "OK 1") this->MoveOk(WEBSITE);
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
    else if (tcpMsgType.left(4) == "test")
    {
        if (QsTcpRespond.left(3) == "OK " && QsTcpRespond.left(4) != "OK 1")
            this->TestOk(); //odpowiedź na testy np. dla zapytania: 'test e2e4' dostaniemy: 'OK e2e4 (...)'.
        else if (QsTcpRespond == "ILLEGAL") //jeżeli test na ruch specjalny się nie powiódł...
            this->MovePiece(WEBSITE, _pChessboard->QsPiecieFromTo); //...to wykonaj zwykły ruch
        else this->wrongTcpAnswer(tcpMsgType, QsTcpRespond);
    }
    else qDebug() << "ERROR: Chess::checkMsgFromChenard() received unknown tcpMsgType: " << tcpMsgType;
}

void WebChess::Promote(QString msg)
{
    _pChessboard->bPromotionConfirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu...
    //...promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->queueMsgs(WEBSITE, "move " + _pChessboard->QsFuturePromote + QStrMsgFromWs.mid(11,1)); //scal żądanie o ruch z żądanym typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Sent to TCP: move " << _pChessboard->QsFuturePromote << QStrMsgFromWs.mid(11,1);
}

void WebChess::NewGame() //przesyłanie prośby o nową grę na TCP
{
    if ((_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
         !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) || //jeżeli obaj gracze siedzą przy stole
            _bServiceTests) //albo mamy do czynienia z zapytaniem serwisowym
        //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
    {
        _pWebsockets->addTextToConsole("Sending 'new' game command to tcp \n", 'w');
        _pTCPMsgs->queueMsgs(WEBSITE, "new");
    }
    else _pWebsockets->addTextToConsole("ERROR: Chess::NewGame(): Wrong players names\n", 'w');
}

void WebChess::MoveTcpPiece(QString msg) // żądanie ruchu- przemieszczenie bierki.
{ //TODO: mylne jest wrażenie że ta funckja już wykonuje ruch bierką
    //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    _pWebsockets->addTextToConsole("Sending normal move to tcp: " + msg + "\n", 'w');
    qDebug() << "Sending normal move to tcp: " << msg;
    _pTCPMsgs->queueMsgs(WEBSITE, msg); //zapytaj się tcp o poprawność prośby o ruch
}

void WebChess::Status()
{
    _pWebsockets->addTextToConsole("Sending 'status' command to tcp. \n", 'w');
    qDebug() << "Sending 'status' command to tcp";
    _pTCPMsgs->queueMsgs(nSender, "status");
}

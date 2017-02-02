#include "chess.h"

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs,
             Websockets *pWebsockets, WebTable *pWebTable): _bServiceTests(false)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pWebsockets = pWebsockets;
    _pWebTable = pWebTable;

    _bServiceTests = false;
}

void Chess::normalPieceMovingSequence() //sekwencja normalnego przemieszczanie bierki
{
    qDebug() << "Start normal move sequence";
    _pDobot->gripperOpennedState(true, 'n'); //true == open. TODO: mózg to interpretuje na odwrót
    //pieceFrom/pieceTo jest automatycznie zawsze jako "up". 1st arg. true == pieceTo, else pieceFrom
    _pDobot->pieceFromTo(false, _pChessboard->PieceFrom.Letter, _pChessboard->PieceFrom.Digit, 'n');
    _pDobot->armUpDown(false, 'n'); // true == up
    _pDobot->gripperOpennedState(false, 'n');
    _pDobot->armUpDown(true, 'n');
    _pChessboard->nTransferredPiece = _pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit]; //pochwycono bierkę
    _pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit] = 0; //miejsce ruszanego pionka jest już puste
    _pDobot->pieceFromTo(true, _pChessboard->PieceTo.Letter, _pChessboard->PieceTo.Digit, 'n');
    _pDobot->armUpDown(false, 'n');
    _pDobot->gripperOpennedState(true, 'n');
    _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit] =
            _pChessboard->nTransferredPiece; //nowe miejsce ruszanego pionka jest już teraz zajęte
    _pChessboard->nTransferredPiece = 0; //żadna bierka nie jest już chwytana
    _pDobot->armUpDown(true, 'n');
    emit _pDobot->addTextToDobotConsole("-End of move sequence-\n");
}

void Chess::removePieceSequence() //sekwencja ruchów przy zbijaniu bierki
{
    qDebug() << "Start remove piece sequence";
    _pDobot->gripperOpennedState(true, 'r'); //true == open
    _pDobot->pieceFromTo(true, _pChessboard->PieceTo.Letter, _pChessboard->PieceTo.Digit, 'r');
    _pDobot->armUpDown(false, 'r');
    _pDobot->gripperOpennedState(false, 'r');
    _pDobot->armUpDown(true, 'r');
    _pChessboard->nTransferredPiece = _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit]; //pochwycono bierkę
    _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit] = 0; //miejsce bitego pionka jest już puste
    _pDobot->removePiece();
    _pDobot->armUpDown(false, 'R');
    _pDobot->gripperOpennedState(true, 'r');
    _pDobot->armUpDown(true, 'R');
    _pChessboard->nTransferredPiece = 0;
}

void Chess::castlingMovingSequence()
{
    qDebug() << "Start castling move sequence";
    this->normalPieceMovingSequence(); //wykonaj przemieszczenie królem
    _pChessboard->castlingFindRookToMove(); //podmień pozycje ruszonego króla na pozycję wieży
    this->normalPieceMovingSequence(); //wykonaj przemieszczenie wieżą
}

void Chess::enpassantMovingSequence()
{
    qDebug() << "Start Enpassant move sequence /n";
    this->normalPieceMovingSequence(); //wykonaj normalny ruch. jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.

    int nTempDigitPos = _pChessboard->PieceTo.Digit; // w razie czego zapaiętaj oryginalną wartość pola (cyfry) bijącego
    if (_pWebTable->getWhoseTurn() == "white_turn")
        _pChessboard->PieceTo.Digit -= 1; //pozycja zbijanego czarnego pionka przez pionka białego w jego turze (białego)
    else if (_pWebTable->getWhoseTurn() == "black_turn")
        _pChessboard->PieceTo.Digit += 1; //pozycja zbijanego białego pionka przez pionka czarnego w jego turze (czarnego)
    else
    {
        emit _pDobot->addTextToDobotConsole("Error03!: Wrong turn in enpassant statement: " + _pWebTable->getWhoseTurn() + "/n");
        qDebug() << "Error03!: Chess::enpassantMovingSequence(): Wrong turn: " << _pWebTable->getWhoseTurn() << "/n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    this->removePieceSequence(); //rozpocznij zbijanie ze zmienioną pozycją pionka bitego.
    _pChessboard->PieceTo.Digit = nTempDigitPos; //wróć prewencyjnie do normalnej pozycji

    _pChessboard->bTestEnpassant = false; //wyłączenie, by nie zapętlać testów w odpowiedzi tcp.
}

bool Chess::testPromotion() //sprawdzanie możliwości wystąpienia promocji
{
    if (((_pWebTable->getWhoseTurn() == "white_turn" &&  _pChessboard->PieceFrom.Digit == 6 &&
          _pChessboard->PieceTo.Digit == 7) ||  //jeżeli bierka chce isć z pola 7 na 8 w turze białego...
         (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->PieceFrom.Digit == 1 &&
          _pChessboard->PieceTo.Digit == 0)) && //...lub z pola 1 na 0 w turze czarnego...
            abs(_pChessboard->PieceFrom.Letter - _pChessboard->PieceTo.Letter) <= 1) //... i ruch odbywa się max o 1 pole odległości...
        //...liter po ukosie (0 dla zwykłego ruchu, 1 dla bicia)
        //TODO: mogę jeszcze wklepać warunek że to ma być pionek (bodajże nr bierki >8 i <25)
    {
        _pChessboard->bTestPromotion = true;
        _pChessboard->QsFuturePromote = _pChessboard->QsPiecieFromTo; //póki nie wiadomo na co promujemy to zapamiętaj zapytanie o ten ruch
        _pTCPMsgs->doTcpConnect("test " + _pChessboard->QsPiecieFromTo + "q"); //test awansu na cokolwiek (królową), który pójdzie na chenard
        //odpowiedź z tcp będzie miała formę: "Ok e2e4 (...)" lub "ILLEGAL"
        return 1;
    }
    else return 0;
}

bool Chess::testEnpassant() //sprawdzanie możliwości wystąpienia enpassant
{
    //sprawdzamy czy zapytanie/ruch może być biciem w przelocie
    if (abs(_pChessboard->PieceFrom.Letter - _pChessboard->PieceTo.Letter) == 1  //jeżeli pionek nie idzie po prostej (tj. po ukosie o 1 pole)...
            && ((_pWebTable->getWhoseTurn() == "white_turn" && _pChessboard->PieceFrom.Digit == 4
                 && _pChessboard->PieceTo.Digit == 5)  //... i jeżeli bije biały z pola 5 na 6
                || (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->PieceFrom.Digit == 3
                    && _pChessboard->PieceTo.Digit == 2))  //... lub czarny bije z pola 4 na 3
            //... i pole na które idzie pionek nie jest zajete (inaczej byłoby to zwykłe bicie, a nie bicie w przelocie)
            && _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceFrom.Digit] == 0)
        //TODO: mogę jeszcze wklepać warunek że to ma być pionek (bodajże nr bierki >8 i <25)
    {
        _pChessboard->bTestEnpassant = true; //przy sprawdzaniu odpowiedzi z tcp warunek wpadnie na tą flagę, zamiast na normalny ruch
        _pTCPMsgs->doTcpConnect("test " + _pChessboard->QsPiecieFromTo);
        return 1; //możemy mieć do czynienia z enpassant
    }
    else return 0;
}

void Chess::checkMsgFromChenard(QString QStrMsgFromChenardTcp)
{
    qDebug() << "Chess::checkMsgFromChenard: " << QStrMsgFromChenardTcp;
    if (QStrMsgFromChenardTcp == "OK 1\n") this->MoveOk();
    else if (QStrMsgFromChenardTcp == "OK\n") this->GameStarted();
    else if (QStrMsgFromChenardTcp.left(3) == "OK " && QStrMsgFromChenardTcp.left(4) != "OK 1")
        this->TestOk(); //odpowiedź na testy np. dla zapytania: 'test e2e4' dostaniemy: 'OK e2e4 (...)'.
    else if (QStrMsgFromChenardTcp.left(7) == "ILLEGAL") this->MovePiece(_pChessboard->QsPiecieFromTo); //jednak to będzie zwykły ruch
    else if (QStrMsgFromChenardTcp.left(8) == "BAD_MOVE") this->BadMove(QStrMsgFromChenardTcp);
    else if (QStrMsgFromChenardTcp.left(1) == "*") this->GameInProgress();
    else if (QStrMsgFromChenardTcp.left(3) == "1-0" || QStrMsgFromChenardTcp.left(3) == "0-1" ||
             QStrMsgFromChenardTcp.left(7) == "1/2-1/2") this->EndOfGame(QStrMsgFromChenardTcp);
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromChenard: " << QStrMsgFromChenardTcp;
}

void Chess::checkMsgFromWebsockets(QString QStrMsgFromWebsockets)
{
    qDebug() << "Chess::checkMsgFromWebsockets: received: " << QStrMsgFromWebsockets;
    if (QStrMsgFromWebsockets == "new") this->NewGame();
    else if (QStrMsgFromWebsockets.left(4) == "move") this->TestMove(QStrMsgFromWebsockets); //sprawdź najpierw czy nie występują ruchy specjalne
    else if (QStrMsgFromWebsockets.left(10) == "promote_to") this->Promote(QStrMsgFromWebsockets); //odp. z WWW odnośnie tego na co promujemy
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromWebsockets: " << QStrMsgFromWebsockets;
}

void Chess::MoveOk() //ruch sie powiódł. wykonaj ruch i zapytaj się tcp o stan gry
{
    qDebug() << "-Begin move sequence-";
    emit this->addTextToDobotConsole("-Begin move sequence-\n");

    if (_pChessboard->castlingStatements()) this->castlingMovingSequence(); //jeżeli warunki dla roszady spełnione to rozpocznij roszadę
    //TODO: brak warunku bicia dla enpassant
    else if (!_pChessboard->bPromotionConfirmed && _pChessboard->removeStatements()) //jeżeli warunki dla bicia spełnione, ale nie dla promocji...
    {
        this->removePieceSequence(); //... to rozpocznij zbijanie ...
        this->normalPieceMovingSequence(); //... a potem wykonaj normalny ruch
    }
    else if (_pChessboard->bPromotionConfirmed) //jeżeli tcp potwierdził wykonanie promocji to rozpocznij promocję (normalny ruch z ew. biciem).
    {
        qDebug() << "-Start promotion move sequence-";
        if (_pChessboard->removeStatements()) //jeżeli przy promocji następuje bicie
            this->removePieceSequence(); //to najpierw zbijamy
        this->normalPieceMovingSequence(); //promocja bez podmiany bierek na inne to póki co zwykły ruch
        _pChessboard->bTestPromotion = false; //czyszczenie zmiennych
        _pChessboard->bPromotionConfirmed = false;

    }
    else this->normalPieceMovingSequence(); //jak nie występuje specjalny ruch, to rozpocznij normalny ruch

    //ruch niech się wykonuje, a w ten czas niech gra sprawdzi czy to nie jest koniec gry komendą 'status'
    _pWebsockets->addTextToWebsocketConsole("Sending 'status' command to tcp. \n");
    qDebug() << "Sending 'status' command to tcp";
    _pTCPMsgs->doTcpConnect("status");
}

void Chess::NewGame() //przesyłanie prośby z WWW o nową grę na TCP
{
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        _pTCPMsgs->setBlokadaZapytan(true);
        if ((_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
                !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) || //jeżeli obaj gracze siedzą przy stole
                _bServiceTests) //albo jeżeli mamy do czynienia z zapytaniem serwisowym
            //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
        {
            _pWebsockets->addTextToWebsocketConsole("Sending 'new' game command to tcp. \n");
            _pTCPMsgs->doTcpConnect("new");
        }
        else _pWebsockets->addTextToWebsocketConsole("Error09! Wrong players names.\n");
    }
    else _pWebsockets->addTextToWebsocketConsole("Error10! Previous msg hasn't been processed.\n");
}

void Chess::GameStarted() //prześlij info na stronę o tym że gra wystartowała
{
    _pWebsockets->addTextToWebsocketConsole("new_game");
    qDebug() << "Sending 'new_game' to site";
    if (!_bServiceTests)_pWebsockets->processWebsocketMsg("new_game");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył wiadomość. uwolnienie blokady zapytań.
}

void Chess::TestMove(QString QStrMsgFromWebsockets)
{
    _pChessboard->findBoardPos(QStrMsgFromWebsockets); //oblicz wszystkie pozycje bierek

    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    { //warunki w testach się znoszą, więc nie trzeba sprawdzać czy wykonają się oba testy.
        if(this->testPromotion()) return; //jeżeli możemy mieć doczynienia z promocją, to sprawdź tą opcję i przerwij jeśli test się powiódł.
        else if (this->testEnpassant()) return; //jeżeli możemy mieć doczynienia z enpassant, to sprawdź tą opcję i przerwij jeśli test się powiódł.
        else MovePiece(QStrMsgFromWebsockets); //jeżeli nie mamy doczynienia ze specjalnymi ruchami, to wykonuj zwykły ruch, roszadę lub bicie.
    }
    else
    {
        _pWebsockets->addTextToWebsocketConsole("Error08! Previous TCP msg hasn't been processed.\n");
        qDebug() << "Error08! Previous TCP msg hasn't been processed.\n";
    }
}

void Chess::MovePiece(QString QStrMsgFromWebsockets) // rządzanie ruchu- przemieszczenie bierki.
{ //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    if (_pTCPMsgs->getBlokadaZapytan() == false) //jeżeli chenard nie przetwarza żadnego zapytania
        //TODO: program gdzieś musi stackować zapytania których nie może jeszcze przetworzyć
    {
        _pWebsockets->addTextToWebsocketConsole("Sending normal move to tcp: " + QStrMsgFromWebsockets + "\n");
        qDebug() << "Sending normal move to tcp: " << QStrMsgFromWebsockets;
        _pTCPMsgs->doTcpConnect(QStrMsgFromWebsockets); //zapytaj się tcp o poprawność prośby o ruch
    }
    {
        _pWebsockets->addTextToWebsocketConsole("Error08! Previous TCP msg hasn't been processed.\n");
        qDebug() << "Error18! Previous TCP msg hasn't been processed.\n";
    }
}

void Chess::Promote(QString QStrMsgFromWs)
{
    _pChessboard->bPromotionConfirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->doTcpConnect("move " + _pChessboard->QsFuturePromote + QStrMsgFromWs.mid(11,1)); //scal żądanie o ruch z żądanym typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Sent to TCP: move " << _pChessboard->QsFuturePromote << QStrMsgFromWs.mid(11,1);
}

void Chess::TestOk()
{
    if(_pChessboard->bTestPromotion) //jeżeli to test na promocję wg tcp się powiódł
    {
        qDebug() << "Sending 'promote_to_what' to websockets";
        _pWebsockets->processWebsocketMsg("promote_to_what"); //to trzeba jeszcze zapytać się na WWW na co ma być ta promocja.
        _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył wiadomość. uwolnienie blokady zapytań.
    }
    else if (_pChessboard->bTestEnpassant)
        this->enpassantMovingSequence(); //jeżeli test na enpassant się powiódł, to rozpocznij ten ruch
    else qDebug() << "Error: Chess::TestOk: None test is true.";
}

void Chess::BadMove(QString QsMsgFromChenardTcp)
{
    QsMsgFromChenardTcp = QsMsgFromChenardTcp.mid(9);
    qDebug() << "Sending 'BAD_MOVE...' to websockets";
    _pWebsockets->processWebsocketMsg("BAD_MOVE " + QsMsgFromChenardTcp.simplified());
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
}

void Chess::GameInProgress() //gra w toku
{
    //podaj na stronę info o tym że ruch został wykonany
    //TODO: sprawdzić czy strona się nie wykrzaczy, bo usunąłem potwierdzenie na stronę...
    //...o tym jaki ruch został wykonany
    qDebug() << "Chess::GameInProgress(): Sending to Websockets: game_in_progress " << _pChessboard->QsPiecieFromTo;
    _pWebsockets->processWebsocketMsg("game_in_progress " + _pChessboard->QsPiecieFromTo);
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
}
void Chess::EndOfGame(QString QStrMsgFromChenardTcp)
{
    QString QsWhoWon;
    if (QStrMsgFromChenardTcp.left(3) == "1-0") QsWhoWon = "white_won";
    else if (QStrMsgFromChenardTcp.left(3) == "0-1") QsWhoWon = "black_won";
    else if (QStrMsgFromChenardTcp.left(7) == "1/2-1/2") QsWhoWon = "draw";
    qDebug() << "Sending '" << QsWhoWon << "' to websockets";
    _pWebsockets->processWebsocketMsg("QsWhoWon");
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
    QsWhoWon.clear();
}

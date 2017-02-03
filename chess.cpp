#include "chess.h"

#define ROW 1
#define COLUMN 0

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
    _pDobot->gripperOpennedState(true, 'n'); //true == open.
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
    _pDobot->pieceFromTo(true, _pChessboard->PieceTo.Letter, _pChessboard->PieceTo.Digit, 'r'); //do usunięnia
    _pDobot->armUpDown(false, 'r');
    _pDobot->gripperOpennedState(false, 'r');
    _pDobot->armUpDown(true, 'r');
    _pChessboard->nTransferredPiece = _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit]; //pochwycono bierkę
    _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit] = 0; //miejsce bitego pionka jest już puste
    _pDobot->removePiece(this->fieldNrToFieldPos(_pChessboard->nTransferredPiece, true),
                         this->fieldNrToFieldPos(_pChessboard->nTransferredPiece, false)); //nad zbitym polem
    _pDobot->armUpDown(false, 'R');
    _pDobot->gripperOpennedState(true, 'r');
    _pDobot->armUpDown(true, 'R');
    _pChessboard->abRemoved[this->fieldNrToFieldPos(_pChessboard->nTransferredPiece, true)]
            [this->fieldNrToFieldPos(_pChessboard->nTransferredPiece, false)]
            = _pChessboard->nTransferredPiece; //miejsce na polu zbitych posiada od teraz swoją zbitą bierkę
    _pChessboard->nTransferredPiece = 0; //ramie nie trzyma już żadnej bierki
}

void Chess::restorePieceSequence(int nPieceToRestore) //TODO: ogarnąć zgrabnie zgranie z funkcją findBoardPos
{
    qDebug() << "Start restore piece sequence";
    _pDobot->gripperOpennedState(true, 's'); //true == open
    //TODO: nazewnictwo poniższej funkcji nie pasuje w tym miejscu. tu jest restore
    _pDobot->removePiece(this->fieldNrToFieldPos(nPieceToRestore, true),
                         this->fieldNrToFieldPos(nPieceToRestore, false));  //ustaw się nad zbitą bierkę
    _pDobot->armUpDown(false, 's');
    _pDobot->gripperOpennedState(false, 's');
    _pDobot->armUpDown(true, 's');
    _pChessboard->nTransferredPiece = nPieceToRestore; //pochwycono bierkę
    _pChessboard->abRemoved[this->fieldNrToFieldPos(nPieceToRestore, true)]
            [this->fieldNrToFieldPos(nPieceToRestore, false)] = 0; //miejsce na polu zbitych jest już puste
    _pDobot->pieceFromTo(true, _pChessboard->PieceTo.Letter, _pChessboard->PieceTo.Digit, 's'); //idź na oryginalne pole...
    //...bierki, by ją odstawić na miejsce.
    _pDobot->armUpDown(false, 's');
    _pDobot->gripperOpennedState(true, 's');
    _pDobot->armUpDown(true, 's');
    _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceTo.Digit] = nPieceToRestore; //bierka na swoim miejscu
    _pChessboard->nTransferredPiece = 0; //ramie przeniosło bierkę i już nic nie trzyma
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

void Chess::resetPiecePositions() //dostaliśmy komunikat "end game" albo "new game"
{
    do //wertuj w kółko planszę przestawiając bierki, dopóki...
       //...szachownica nie wróci do stanu pierwotnego/startowego.
    {
        for (int nCurentlyCheckedField = 1; nCurentlyCheckedField <= 64; nCurentlyCheckedField++) //aktualnie sprawdzane pole szachownicy
        {
            int nPiecePos = _pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)] //bierka na polu sprawdzanym
                    [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)];
            if(nPiecePos > 16) nPiecePos += 32;

            //TODO: ogarnąć co mogę w kodzie pomienić na nPiecePos (bardzo łatwo się zajebać...
            //...w odróżnianiu nr bierek i nr pól. przemyśleć to dokładnie zanim zacznę podmieniać).
            if (_pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                    [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)] == 0 //jeżeli na sprawdzanym polu nic nie ma...
                    && (this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1  //...a coś tam powinno być
                        || this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) >= 6)) // (kolumny <= 1 to białe, a >= 6 to czarne)
            {
                int nPieceToFind; //zmienna określanąca nr szukanej bierki
                if (this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1) //jeżeli to jest piewsza lub druga kolumna szachownicy (białe)
                    nPieceToFind = _pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                            [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)]; //to nr bierki (białej) której szukamy = nr sprawdzanego pola
                else nPieceToFind = _pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                        [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)] - 32; //bierki czarne są na polach o nr większych o 32 od tych bierek

                //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                if(_pChessboard->abRemoved[this->fieldNrToFieldPos(nPieceToFind, ROW)]
                        [this->fieldNrToFieldPos(nPieceToFind, COLUMN)] == nPieceToFind) //jeżeli tam jest
                {
                    _pChessboard->findBoardPos(this->fieldNrToFieldPos(nPieceToFind, ROW),
                                               this->fieldNrToFieldPos(nPieceToFind, COLUMN),
                                               this->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                               this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                    this->restorePieceSequence(nPieceToFind); //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                }
                else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy
                {
                    for (int nField = 1; nField <= 64; nField++) //przewertuj szachownicę w jej poszukiwaniu
                    {
                        if (nField == _pChessboard->anBoard[this->fieldNrToFieldPos(nField, ROW)]
                                [this->fieldNrToFieldPos(nField, COLUMN)] //jeżeli na tym polu szachownicy jest bierka której szukamy
                                && nCurentlyCheckedField != nField) //ale nie może to być oryginalna pozycja bierki (musi być na obcym polu startowym)
                        {
                            //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                            _pChessboard->findBoardPos(this->fieldNrToFieldPos(nField, ROW),
                                                       this->fieldNrToFieldPos(nField, COLUMN),
                                                       this->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                       this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                            this->normalPieceMovingSequence();
                        }
                    }
                }
            }
            //natomiast jeżeli na sprawdzanym polu jest bierka, która nie znajdowała się tam podczas startu gry...
            //...to trzeba sprawdzić czy oryginalne pole tej bierki nie jest zajęte i tam ją odstawić
            else if (_pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                     [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)] != nCurentlyCheckedField
                     && _pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                     [this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)] != 0) //i jakaś bierka na pewno tam jest
            {
               /* int nPiecePos = _pChessboard->anBoard[this->fieldNrToFieldPos(nCurentlyCheckedField, true)] //bierka na polu sprawdzanym
                        [this->fieldNrToFieldPos(nCurentlyCheckedField, false)]; //TODO: dać to na start pętli
                if(nPiecePos > 16) nPiecePos += 32;*/

                if (_pChessboard->anBoard[this->fieldNrToFieldPos(nPiecePos, ROW)] //czy oryginalne pole bierki (nie będącej na swojej de facto ...
                        [this->fieldNrToFieldPos(nPiecePos, COLUMN)] == 0) //...pierwonej pozycji) jest puste, by tam można było ją odstawić
                {
                    //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                    _pChessboard->findBoardPos(this->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                               this->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN),
                                               this->fieldNrToFieldPos(nPiecePos, ROW),
                                               this->fieldNrToFieldPos(nPiecePos, COLUMN));
                    this->normalPieceMovingSequence();
                }
                //else: jeżeli oryginalne pole tej bierki też jest zajęte, to niech pętla wykona się jeszcze raz, a...
                //wtedy jej miejsce może być już wolne
            }
            //else: jeśli żadna opcja nie wystąpiła, tzn. że na danym polu leży prawidłowa/startowa bierka...
            //albo bierki nie da się odstawić, bo zajmuje ją aktualnie inna bierka- to sprawdź kolejne pole

            bool bArraysAreEqual = true;
            for (int i=0; i<8; i++) //sprawdzanie czy udało się doprowadzić bierki na szachownicy...
            { //...do stanu początkowego/startowego
                for (int j=0; j<8; j++)
                {
                    //jeżeli którakolwiek bierka nie znajduje się na swoim pierwotnym/oryginalnym polu...
                    if (_pChessboard->anBoard[i][j] != _pChessboard->anStartBoard[i][j])
                        bArraysAreEqual = false; //...to stan rozmieszczenia bierek na szachownicy nie jest ostateczny/prawidłowy.
                }
            }

            bool isChessboardChanged = false;
            for (int i=0; i<8; i++) //sprawdzanie czy ustawienie bierek zmieniło się od ostatniego...
            { //...przejścia przez pętlę przestawiającą bierki.
                for (int j=0; j<8; j++)
                {
                    //jeżeli jest różnica od ostatniego razu, to zapamiętaj że tak jest
                    if (_pChessboard->anBoard[i][j] != _pChessboard->anTempBoard[i][j])
                        isChessboardChanged = true;
                    if (isChessboardChanged = true) break;
                }
                if (isChessboardChanged = true) break;
            }

            if (isChessboardChanged) //jeżeli przewertowane planszy w poszukiwaniu bierek do odstawienia...
            { //...spowodowało przemieszczenie się jakiejś bierki, to zapamiętaj rozmieszczenie bierek...
                //...na szachownicy do sprawdzania tego warunku w kolejnym przejściu pętli.
                for (int i=0; i<8; i++)
                {
                    for (int j=0; j<8; j++)
                    {
                        //zapisz w pamięci aktualny rozkład bierek na szachownicy, by kontrolować...
                        //...czy nie zapętliłem się z warunkiem w do-while.
                        _pChessboard->anTempBoard[i][j] = _pChessboard->anBoard[i][j];
                        //trzeba sprawdzać z każdą pętlą przestawiającą bierki czy stan szachownicy...
                        //...zmienił się od ostatniego przewertowania całej szachownicy. jeżeli nic...
                        //...się nie zmienia, to mogło dojść do sytuacji gdzie 2 bierki stoją na polach...
                        //...podmienionych względem siebie nawzajem. w takiej sytuacji należy jedną...
                        //...bierkę odłożyć na pole zbitych, drugą odstawić na miejsce i pierwszą...
                        //...przywrócić na swoje już puste miejsce na szachownicy.
                    }
                }
            }
            else //w innym wypadku nastąpił (najprawdopodobniej) żadki przypadek, gdzie 2 bierki zajmują...
                //...wzajemnie pole "tej drugiej bierki" przez co nie da się ich przestawić. należy pierwszą...
                //...z brzegu odstawić na pole bierek zbitych, kolejną odstawić na miejsce, by tą pierwszą...
                //...też móc odstawić na swoje pierwotne miejsce startowe.
            {
                //złap pierwszą bierkę z brzegu, która nie jest na swojej pozycji i odstaw ją na...
                //...obszar bierek zbitych.

                //TODO: no i tutaj znowu trzeba użyć rozległych części kodu z tej metody powyżej.
                //ogarnąć najpierw kod powyżej by był czytelny, sprawdzić potem które części kodu
                //będą tutaj przydatne, zapakować je jeszcze czytelniej i wtedy tutaj użyć.
            }
        }
    }
    while (bArraysAreEqual); //wertuj w kółko planszę przestawiając bierki, dopóki...
    //...szachownica nie wróci do stanu pierwotnego/startowego.
}

int Chess::fieldNrToFieldPos(int nfieldNr, bool bRow)
{
    if (nfieldNr != 0) //zabezpieczenie przed przypadkowym podaniem zera do mianownika
    {
        int nfieldNrColumn, nfieldNrRow; //tablica[nfieldNrRow][nfieldNrColumn];

        if (nfieldNr % 8 != 0) //wszystkie prócz liczb (tj. bierek nr) 8, 16, 24 i 32.
        {
            nfieldNrColumn = nfieldNr / 8;
            nfieldNrRow  = (nfieldNr - 1) - (nfieldNrColumn * 8);
        }
        else //dla liczb (tj. bierek nr) 8, 16, 24 i 32.
        {
            nfieldNrColumn = (nfieldNr / 8) - 1;
            nfieldNrRow = 7;
        }

        if (bRow) return nfieldNrRow ;
        else return nfieldNrColumn;
    }
    else
    {
        emit this->addTextToDobotConsole("ERROR. Chess::fieldNrToFieldPos: próba dzielenia przez zero");
        qDebug() << "ERROR. Chess::fieldNrToFieldPos: proba dzielenia przez zero";
        return 0; //coś trzeba zwrócić
    }
}

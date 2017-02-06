#include "chess.h"

#define ROW 1
#define COLUMN 0

#define OPEN 1
#define CLOSE 0

#define FROM 1
#define TO 0

#define UP 1
#define DOWN 0

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

//TODO: 3 poniższe sekwencje + findBoardPos() da się tu upchać
void Chess::pieceMovingSequence(char chMoveType, int nPieceFromLetter, int nPieceFromDigit,
                                int nPieceToLetter, int nPieceToDigit)
{
    QString QsSequenceType;
    switch (chMoveType)
    {
    case 'n': QsSequenceType = "normal"; break; //ruch z szachownicy na szachownicę (zwykły)
    case 'c': QsSequenceType = "castling(king)"; break; //ruch z szachownicy na szachownicę (roszada królem)
    case 'o': QsSequenceType = "castling(rook)"; break; //ruch z szachownicy na szachownicę (roszada wieżą)
    case 'e': QsSequenceType = "enpassant"; break; //ruch z szachownicy na szachownicę (enpassant)
    case 'p': QsSequenceType = "promotion"; break; //ruch z szachownicy na szachownicę (promocja)
    case 'r': QsSequenceType = "remove"; break; //ruch z szachownicy na obszar zbitych bierek
    case 's': QsSequenceType = "restore"; break; //ruch z obszaru zbitych bierek na szachownicę
    default: QsSequenceType = "ERROR: Chess::pieceMovingSequence: unknown chMoveType"; break;
    }
    qDebug() << "Start" << QsSequenceType << "move sequence.";

    if (nPieceFromLetter == -1) nPieceFromLetter = _pChessboard->PieceFrom.Letter;
    if (nPieceFromDigit == -1) nPieceFromDigit = _pChessboard->PieceFrom.Digit;
    if (nPieceToLetter == -1) nPieceToLetter = _pChessboard->PieceTo.Letter;
    if (nPieceToDigit == -1) nPieceToDigit = _pChessboard->PieceTo.Digit;

    if (chMoveType == 'r')
    {
        nPieceFromLetter = nPieceToLetter; //przy usuwaniu bierka "from" to ta...
        nPieceFromDigit = nPieceToDigit; //...która w każdym innym ruchu jest jako "to"
    }

    _pDobot->gripperOpennedState(OPEN, chMoveType);
    _pDobot->pieceFromTo(FROM, nPieceFromLetter, nPieceFromDigit, chMoveType);
    _pDobot->armUpDown(DOWN, FROM, chMoveType);
    _pDobot->gripperOpennedState(CLOSE, chMoveType);
    _pDobot->armUpDown(UP, FROM, chMoveType);
    _pChessboard->pieceStateChanged(FROM, nPieceFromLetter, nPieceFromDigit, chMoveType);

    _pDobot->pieceFromTo(TO, nPieceToLetter, nPieceToDigit, chMoveType);
    _pDobot->armUpDown(DOWN, TO, chMoveType);
    _pDobot->gripperOpennedState(OPEN, chMoveType);
    _pDobot->armUpDown(UP, TO, chMoveType);
    _pChessboard->pieceStateChanged(TO, nPieceToLetter, nPieceToDigit, chMoveType);

    qDebug() << "-End of move sequence-";
    emit this->addTextToDobotConsole("-End of move sequence-\n");
}

void Chess::castlingMovingSequence()
{
    this->pieceMovingSequence('c'); //wykonaj przemieszczenie królem
    _pChessboard->castlingFindRookToMove(); //podmień pozycje ruszonego króla na pozycję wieży
    this->pieceMovingSequence('o'); //wykonaj przemieszczenie wieżą
}

void Chess::enpassantMovingSequence()
{
    qDebug() << "Start Enpassant move sequence /n";
    //wykonaj normalny ruch. enpassant to jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.
    this->pieceMovingSequence('e');

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
    this->pieceMovingSequence('r'); //...zmienioną pozycją pionka bitego.
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
    else if (QStrMsgFromWebsockets.left(5) == "reset") this->resetPiecePositions(); //przywróć bierki na szachownicę do stanu startowego
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromWebsockets: " << QStrMsgFromWebsockets;
}

void Chess::MoveOk() //ruch sie powiódł. wykonaj ruch i zapytaj się tcp o stan gry
{
    qDebug() << "-Begin move sequence-";
    emit this->addTextToDobotConsole("-Begin move sequence-\n");

    if (_pChessboard->castlingStatements()) //jeżeli warunki dla roszady spełnione...
        this->castlingMovingSequence(); //...to rozpocznij roszadę
    //!TODO: brak warunku bicia dla enpassant
    else if (_pChessboard->removeStatements()  //jeżeli warunki dla bicia są spełnione...
             && !_pChessboard->bPromotionConfirmed) //..., ale nie są spełnione dla promocji...
    {
        this->pieceMovingSequence('r'); //... to rozpocznij zbijanie ...
        this->pieceMovingSequence('n'); //... a potem wykonaj normalny ruch
    }
    else if (_pChessboard->bPromotionConfirmed) //jeżeli tcp potwierdził wykonanie promocji to rozpocznij promocję (normalny ruch z ew. biciem).
    {
        qDebug() << "-Start promotion move sequence-";
        if (_pChessboard->removeStatements()) //jeżeli przy promocji następuje bicie
            this->pieceMovingSequence('r'); //to najpierw zbijamy
        this->pieceMovingSequence('n'); //promocja bez podmiany bierek na inne to póki co zwykły ruch
        _pChessboard->bTestPromotion = false; //czyszczenie zmiennych
        _pChessboard->bPromotionConfirmed = false;

    }
    else this->pieceMovingSequence('n'); //jak nie występuje specjalny ruch, to rozpocznij normalny ruch

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
    _pWebsockets->processWebsocketMsg(QsWhoWon);
    _pTCPMsgs->setBlokadaZapytan(false); //chenard przetworzył właśnie wiadomość. uwolnienie blokady zapytań
    QsWhoWon.clear();

    this->resetPiecePositions(); //przywróć wszystkie bierki do stanu początkowego
    //TODO: zablokować możliwość robienia czegokolwiek na stronie aż to się nie zakończy
}

void Chess::resetPiecePositions() //dostaliśmy komunikat "end game" albo "new game"
{
    //TODO: sprawdzić czy nie zapcham pamięci urządzenia ogromną ilością zapytań o ruch

    if (!_pChessboard->compareArrays(_pChessboard->anBoard, _pChessboard->anStartBoard))
        //jeżeli bierki stoją na szachownicy nieruszone, to nie ma potrzeby ich odstawiać
    {
        qDebug() << "Chess::resetPiecePositions()";

        bool  bArraysEqual; //warunek wyjścia z poniższej pętli do..for
        bool isChessboardInvariable; //do sprawdzania czy pętla do..for się nie zacieła

        do //wertuj w kółko planszę przestawiając bierki, dopóki...
            //...szachownica nie wróci do stanu pierwotnego/startowego.
        {
            for (int nCurentlyCheckedField = 1; nCurentlyCheckedField <= 64; nCurentlyCheckedField++) //aktualnie sprawdzane pole szachownicy
            {
                //nr bierki która jest na aktualnie sprawdzanym polu (1-32)
                int nPieceTypeOnCheckedField = _pChessboard->anBoard
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)];

                //nr bierki która POWINNA BYĆ na aktualnie sprawdzanym polu (1-32)
                int nStartPieceTypeOnCheckedField = _pChessboard->anStartBoard
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)];

                //nr oryginalnego/pierwotnego pola, na którym powinna znajdować...
                //...się bierka będąca na aktualnie sprawdzanym polu (1-16, 49-64)
                int nPieceOrigPos;
                if (nPieceTypeOnCheckedField == 0) nPieceOrigPos = 0;
                else if (nPieceTypeOnCheckedField >16) nPieceOrigPos = nPieceTypeOnCheckedField + 32; //dla czarnych bierek (17-32) są to pola 49-64
                else nPieceOrigPos = nPieceTypeOnCheckedField; //dla białych bierek pole oryginalne/pierwotne = nr bierki

                //nr bierki na zbitym polu odpowiadającym bierce na polu aktualnie sprawdzanym (1-32)
                int nRemovedFieldPieceType = 0;
                if (nPieceTypeOnCheckedField != 0) //nie sprawdzamy pola bierki zbitej, jeżeli nie mamy doczynienia z żadną bierką na polu sprawdzanym.
                {
                    nRemovedFieldPieceType = _pChessboard->anRemoved
                            [_pChessboard->fieldNrToFieldPos(nPieceTypeOnCheckedField, ROW)]
                            [_pChessboard->fieldNrToFieldPos(nPieceTypeOnCheckedField, COLUMN)];
                }

                if (nPieceTypeOnCheckedField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1  //...a coś tam powinno być
                            || _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) >= 6)) // (kolumny <= 1 to białe, a >= 6 to czarne)
                {
                    qDebug() << "Chess::resetPiecePositions(): nPieceTypeOnCheckedField == 0";
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(nRemovedFieldPieceType == nStartPieceTypeOnCheckedField) //jeżeli na polu zbitych jest bierka, która powinna się znajdywać...
                        //...na aktualnie sprawdzanym polu szachownicy
                    {
                        qDebug() << "Chess::resetPiecePositions(): nRemovedFieldPieceType == nStartPieceTypeOnCheckedField";
                        this->pieceMovingSequence('s', //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, COLUMN),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy
                    {
                        qDebug() << "Chess::resetPiecePositions(): nRemovedFieldPieceType != nStartPieceTypeOnCheckedField";
                        for (int nField = 1; nField <= 64; nField++) //przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nStartPieceTypeOnCheckedField == _pChessboard->anBoard
                                    [_pChessboard->fieldNrToFieldPos(nField, ROW)]
                                    [_pChessboard->fieldNrToFieldPos(nField, COLUMN)] //jeżeli na tym wertowanym polu szachownicy jest bierka której szukamy
                                    && nCurentlyCheckedField != nField) //ale nie może to być oryginalna pozycja bierki (musi być na obcym polu startowym)
                            {
                                //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                                this->pieceMovingSequence('n',_pChessboard->fieldNrToFieldPos(nField, ROW),
                                                          _pChessboard->fieldNrToFieldPos(nField, COLUMN),
                                                          _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                          _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                            }
                        }
                    }
                }
                //natomiast jeżeli na sprawdzanym polu jest bierka, która nie znajdowała się tam podczas startu gry...
                //...to trzeba sprawdzić czy oryginalne pole tej bierki nie jest zajęte i tam ją odstawić
                else if (nPieceTypeOnCheckedField != 0 && //jeżeli jakaś bierka na pewno tam jest
                         nStartPieceTypeOnCheckedField != nPieceTypeOnCheckedField) // i nie to bierka która jest tam orygnalnie
                {
                    if (nPieceOrigPos == 0) //czy startowe pole bierki jest puste, by tam można było ją odstawić
                    {
                        qDebug() << "Chess::resetPiecePositions(): nPieceTypeOnCheckedField != 0 && nPieceOrigPos == 0";

                        //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                        this->pieceMovingSequence('n', _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN),
                                                  _pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN));
                    }
                    else //jeżeli startowe pole bierki na polu sprawdzanym jest zajęte przez inną bierkę
                    {
                        int nNrOf2ndPieceOn1stPieceOrigPos = //bierka znajdująca się na polu starowym bierki będącej na...
                                _pChessboard->anBoard[_pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW)] //...polu aktualnie sprawdzanym.
                                [_pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN)];

                        int nStartNrOf2ndPieceOn1stPieceOrigPos = _pChessboard->anStartBoard //pole startowe bierki znajdującej się na polu...
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW)] //...starowym bierki, która jest...
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN)]; //...na polu aktualnie sprawdzanym.

                        if (nStartNrOf2ndPieceOn1stPieceOrigPos == nCurentlyCheckedField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            qDebug() << "Chess::resetPiecePositions(): nStartNrOf2ndPieceOn1stPieceOrigPos == nCurentlyCheckedField";

                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            this->pieceMovingSequence('r', _pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN),
                                                      _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN));

                            //teraz przenieś bierkę z pola aktualnie sprawdzanego, na pole bierki dopiero co odłożonej na obszar bierek zbitych
                            this->pieceMovingSequence('n', _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN));

                            //na końcu bierkę odłożoną poza planszę przywróć na pole bierki wcześniej przestawionej
                            this->pieceMovingSequence('s', _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN),
                                                      _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                        }
                    }
                    //everything else: jeżeli oryginalne pole bierki na polu sprawdzanym też jest zajęte, a nie...
                    //...jest to bierka, której pozycja startowa to pole aktualnie sprawdzaneto niech pętla wykona...
                    //...się jeszcze raz, a wtedy układ bierek się na pewno zmieni i będzie dało się dalej coś przestawiać.
                }
                //else: jeśli żadna opcja nie wystąpiła, tzn. że na danym polu leży prawidłowa/startowa bierka...
                //...albo bierki nie da się odstawić, to sprawdź kolejne pole
            }
            //sprawdzanie czy ustawienie bierek na szachownicy jest ostateczne, tj. takie same jak na...
            //...starcie. Jeżeli którakolwiek bierka nie znajduje się na swoim startowym polu, to stan...
            //...rozmieszczenia bierek na szachownicy nie jest ostateczny.
            bArraysEqual = _pChessboard->compareArrays(_pChessboard->anBoard,
                                                       _pChessboard->anStartBoard);

            if (!bArraysEqual) //jeżeli plansza jest w stanie startowym, to można nie robić reszty...
                //...poleceń i wyskoczyć z pętli
            {
                qDebug() << "Chess::resetPiecePositions(): anBoard != anStartBoard";

                //sprawdzanie czy ustawienie bierek zmieniło się od ostatniego przejścia przez pętlę...
                //...przestawiającą bierki. Jeżeli jest różnica od ostatniego razu, to zapamiętaj że...
                //...tak jest. Pozwoli to sprawdzać czy pętla do..for nie zacięła się, a co za tym...
                //...idzie- spradzane jest czy odstawianie bierek na planszę się nie zacieło.
                isChessboardInvariable = _pChessboard->compareArrays(_pChessboard->anBoard,
                                                                     _pChessboard->anTempBoard);

                //jeżeli aktualne przewertowane planszy w poszukiwaniu bierek do odstawienia...
                //...spowodowało przemieszczenie się jakiejś bierki, to zapamiętaj rozmieszczenie...
                //...bierek na szachownicy do sprawdzania tego warunku w kolejnym przejściu pętli.
                if (!isChessboardInvariable)
                {
                    memcpy(_pChessboard->anTempBoard, _pChessboard->anBoard,
                           sizeof(_pChessboard->anBoard)); //anTempBoard = anBoard
                }
                else
                {
                    qDebug() << "ERROR: Chess::resetPiecePositions() stuck- shouldn't be possible. isChessboardInvariable ="
                             << isChessboardInvariable;
                    bArraysEqual = true; //wyjdź z pętli, bo inaczej program się tutaj zatnie
                }
            }
            else qDebug() << "Chess::resetPiecePositions(): anBoard == anStartBoard";
        }
         //wertuj w kółko planszę przestawiając bierki, dopóki
        //...szachownica nie wróci do stanu pierwotnego/startowego.
        while (!bArraysEqual); //wykonuj pętlę dopóki warunek jest prawdziwy

        qDebug() << "End of: Chess::resetPiecePositions()";
    }
}

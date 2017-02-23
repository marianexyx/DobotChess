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
    m_bAI = false;
}

void Chess::pieceMovingSequence(char chMoveType, int nPieceFromLetter, int nPieceFromDigit,
                                int nPieceToLetter, int nPieceToDigit)
{
    _pDobot->writeMoveTypeInConsole(chMoveType);

    //jeżeli nie podano skąd i/lub dokąd, to jest to ruch z aktualnego pola szachownicy nad którym wisi ramie
    if (nPieceFromLetter == -1) nPieceFromLetter = _pChessboard->PieceFrom.Letter;
    if (nPieceFromDigit == -1) nPieceFromDigit = _pChessboard->PieceFrom.Digit;
    if (nPieceToLetter == -1) nPieceToLetter = _pChessboard->PieceTo.Letter;
    if (nPieceToDigit == -1) nPieceToDigit = _pChessboard->PieceTo.Digit;

    if (chMoveType == 'r')
    {
        nPieceFromLetter = nPieceToLetter; //przy usuwaniu bierka "from" to ta...
        nPieceFromDigit = nPieceToDigit; //...która w każdym innym ruchu jest jako "to"
    }

    qDebug() << "-Start move sequence-";
    emit this->addTextToDobotConsole("-End of move sequence-\n");

    _pDobot->gripperOpennedState(OPEN, chMoveType);
    _pDobot->pieceFromTo(FROM, nPieceFromLetter, nPieceFromDigit, chMoveType);
    _pDobot->armUpDown(DOWN, FROM, chMoveType);
    _pDobot->gripperOpennedState(CLOSE, chMoveType);
    _pDobot->wait(300);
    _pDobot->armUpDown(UP, FROM, chMoveType);
    _pChessboard->pieceStateChanged(FROM, nPieceFromLetter, nPieceFromDigit, chMoveType);

    _pDobot->pieceFromTo(TO, nPieceToLetter, nPieceToDigit, chMoveType);
    _pDobot->armUpDown(DOWN, TO, chMoveType);
    _pDobot->gripperOpennedState(OPEN, chMoveType);
    _pDobot->wait(300);
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
    //wykonaj normalny ruch. enpassant to jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.
    this->pieceMovingSequence('e');

    int nTempDigitPos = _pChessboard->PieceTo.Digit; //w razie czego zapaiętaj oryginalną wartość pola (cyfry) bijącego
    if (_pWebTable->getWhoseTurn() == "white_turn")
        _pChessboard->PieceTo.Digit -= 1; //pozycja zbijanego czarnego pionka przez pionka białego w jego turze (białego)
    else if (_pWebTable->getWhoseTurn() == "black_turn")
        _pChessboard->PieceTo.Digit += 1; //pozycja zbijanego białego pionka przez pionka czarnego w jego turze (czarnego)
    else
    {
        emit _pDobot->addTextToDobotConsole("Error03!: Wrong turn in enpassant statement: "
                                            + _pWebTable->getWhoseTurn() + "/n");
        qDebug() << "Error03!: Chess::enpassantMovingSequence(): Unknown turn type: "
                 << _pWebTable->getWhoseTurn() << "/n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    this->pieceMovingSequence('r'); //usuń pionka bitego ze  zmienioną pozycją.
    _pChessboard->PieceTo.Digit = nTempDigitPos; //wróć prewencyjnie w pamięci do normalnej pozycji

    //TODO: te testy mogłyby by być bardziej na poziomie tcp, tzn odpowiedź z tcp mogłaby być...
    //...modyfikowana jeszcze przed wyjściem z ciała obiektu tcp, tak by te flagi tam były wyłączane.
    _pChessboard->bTestEnpassant = false; //wyłączenie, by nie zapętlać testów w odpowiedzi tcp.
}

bool Chess::testPromotion() //sprawdzanie możliwości wystąpienia promocji
{
    //poniżym warunkiem eliminuję jak największą ilość przypadkowych zapytań testowych do tcp o promocję
    if (((_pWebTable->getWhoseTurn() == "white_turn" &&  _pChessboard->PieceFrom.Digit == 6
          && _pChessboard->PieceTo.Digit == 7   //jeżeli w turze białego bierka chce isć z pola 7 na 8...
          && (_pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit] > 8 //...i jeżeli ruszany to...
              && _pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit] <= 16)) //...biały pionek...
         || (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->PieceFrom.Digit == 1
             && _pChessboard->PieceTo.Digit == 0    //...lub z pola 1 na 0 w turze czarnego...
             && _pChessboard->anBoard[_pChessboard->PieceFrom.Letter]
             [_pChessboard->PieceFrom.Digit] > 48)) //...i jeśli jest to czarny pionek...
            && abs(_pChessboard->PieceFrom.Letter - _pChessboard->PieceTo.Letter) <= 1) //... i ruch odbywa się max o 1 pole odległości...
        //...liter po ukosie (0 dla zwykłego ruchu, 1 dla bicia)
    {
        _pChessboard->bTestPromotion = true;
        _pChessboard->QsFuturePromote = _pChessboard->QsPiecieFromTo; //póki nie wiadomo na co promujemy to zapamiętaj zapytanie o ten ruch
        _pTCPMsgs->queueMsgs("test " + _pChessboard->QsPiecieFromTo + "q"); //test awansu na cokolwiek (królową), który pójdzie na chenard
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
                 && _pChessboard->PieceTo.Digit == 5  //... i jeżeli bije biały z pola 5 na 6
                 && (_pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit] > 8 //...i jeżeli ruszany to...
                     && _pChessboard->anBoard[_pChessboard->PieceFrom.Letter][_pChessboard->PieceFrom.Digit] <= 16)) //...biały pionek...)
                || (_pWebTable->getWhoseTurn() == "black_turn" && _pChessboard->PieceFrom.Digit == 3
                    && _pChessboard->PieceTo.Digit == 2  //... lub czarny bije z pola 4 na 3...
                && _pChessboard->anBoard[_pChessboard->PieceFrom.Letter]
                [_pChessboard->PieceFrom.Digit] > 48)) //...i jeśli jest to czarny pionek...
            //... i pole na które idzie pionek nie jest zajete (inaczej byłoby to zwykłe bicie, a nie bicie w przelocie)
            && _pChessboard->anBoard[_pChessboard->PieceTo.Letter][_pChessboard->PieceFrom.Digit] == 0)
    {
        _pChessboard->bTestEnpassant = true; //przy sprawdzaniu odpowiedzi z tcp warunek wpadnie na tą flagę, zamiast na normalny ruch
        _pTCPMsgs->queueMsgs("test " + _pChessboard->QsPiecieFromTo);
        return 1; //możemy mieć do czynienia z enpassant
    }
    else return 0;
}

void Chess::checkMsgFromChenard(QString QsTcpRespond)
{
    //TODO: ruchy z MoveOk i TestOk wykonywać w jednej funkcji dla porządku kodu
    qDebug() << "Chess::checkMsgFromChenard: " << QsTcpRespond;
    if (QsTcpRespond == "OK 1\n") this->MoveOk();
    else if (QsTcpRespond == "OK\n") this->GameStarted();
    else if (QsTcpRespond.left(3) == "OK " && QsTcpRespond.left(4) != "OK 1")
    {
        if (m_bAI) //jeżeli mamy doczynienia z botem, który wymyślił ruch
            //to zapisz ten ruch w pamięci i czekaj, aż człowiek wywoła ten ruch chęcią zaczęcia gry
            _pChessboard->QsAIPiecieFromTo = QsTcpRespond;
        else
            this->TestOk(); //odpowiedź na testy np. dla zapytania: 'test e2e4' dostaniemy: 'OK e2e4 (...)'.
    }
    else if (QsTcpRespond.left(7) == "ILLEGAL") this->MovePiece(_pChessboard->QsPiecieFromTo); //jednak to będzie zwykły ruch
    else if (QsTcpRespond.left(8) == "BAD_MOVE") this->BadMove(QsTcpRespond);
    else if (QsTcpRespond.left(1) == "*") this->GameInProgress();
    else if (QsTcpRespond.left(3) == "1-0" || QsTcpRespond.left(3) == "0-1" ||
             QsTcpRespond.left(7) == "1/2-1/2") this->EndOfGame(QsTcpRespond);
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromChenard: " << QsTcpRespond;
}

void Chess::checkMsgFromWebsockets(QString msgFromWs)
{
    qDebug() << "Chess::checkMsgFromWebsockets: received: " << msgFromWs;
    if (msgFromWs == "new") this->NewGame();
    else if (msgFromWs.left(4) == "move") this->TestMove(msgFromWs); //sprawdź najpierw czy nie występują ruchy specjalne
    else if (msgFromWs.left(10) == "promote_to") this->Promote(msgFromWs); //odp. z WWW odnośnie tego na co promujemy
    else if (msgFromWs.left(5) == "reset") this->resetPiecePositions(); //przywróć bierki na szachownicę do stanu startowego
    else qDebug() << "ERROR: received not recognized msg in Chess::checkMsgFromWebsockets: " << msgFromWs;
}

void Chess::MoveOk() //ruch w pamięci sie powiódł.
{
    //TODO: mylna jest nazwa funkcji z której możnaby wnioskować, że ruch już się wykonał
    qDebug() << "-Begin move sequence-";
    emit this->addTextToDobotConsole("-Begin move sequence-\n");

    if (_pChessboard->castlingStatements()) //jeżeli warunki dla roszady spełnione...
        this->castlingMovingSequence(); //...to rozpocznij roszadę
    //enpassant wykonuje się w testach
    else if (_pChessboard->removeStatements()  //jeżeli warunki dla bicia są spełnione...
             && !_pChessboard->bPromotionConfirmed) //..., ale nie są spełnione dla promocji...
    {
        this->pieceMovingSequence('r'); //... to rozpocznij zbijanie ...
        this->pieceMovingSequence('n'); //... a potem wykonaj normalny ruch
    }
    else if (_pChessboard->bPromotionConfirmed) //jeżeli tcp potwierdził wykonanie promocji to rozpocznij promocję
    {
        if (_pChessboard->removeStatements()) //jeżeli przy promocji następuje bicie
            this->pieceMovingSequence('r'); //to najpierw zbijamy
        this->pieceMovingSequence('p'); //promocja bez podmiany bierek na inne to póki co zwykły ruch
        _pChessboard->bTestPromotion = false; //czyszczenie zmiennych
        _pChessboard->bPromotionConfirmed = false;

    }
    else this->pieceMovingSequence('n'); //jak nie występuje specjalny ruch, to rozpocznij normalny ruch

    //ruch niech się wykonuje, a w ten czas niech gra sprawdzi czy to nie jest koniec gry komendą 'status'
    _pWebsockets->addTextToWebsocketConsole("Sending 'status' command to tcp. \n");
    qDebug() << "Sending 'status' command to tcp";
    _pTCPMsgs->queueMsgs("status");
}

void Chess::NewGame() //przesyłanie prośby z WWW o nową grę na TCP
{
    if ((_pWebTable->getNameWhite() != "Biały" && _pWebTable->getNameBlack() != "Czarny" && //zabezpieczenie:
         !_pWebTable->getNameWhite().isEmpty() && !_pWebTable->getNameBlack().isEmpty()) || //jeżeli obaj gracze siedzą przy stole
            _bServiceTests || m_bAI) //albo mamy do czynienia z zapytaniem serwisowym albo botem
        //TODO: dodać więcej zabezpieczeń (inne nazwy, typy itd) i reagować na nie jakoś
    {
        _pWebsockets->addTextToWebsocketConsole("Sending 'new' game command to tcp. \n");
        _pTCPMsgs->queueMsgs("new");
    }
    else _pWebsockets->addTextToWebsocketConsole("Error09! Wrong players names.\n");
}

void Chess::GameStarted() //zareaguj na to że gra wystartowała
{
    if (m_bAI) //jeżeli mamy do czynienia z grą z botem
    {
        _pTCPMsgs->doTcpConnect("think 5000"); //wymyśl pierwszy ruch bota białego
    }
    else //prześlij info na stronę o tym że gra wystartowała
    {
        _pWebsockets->addTextToWebsocketConsole("new_game");
        qDebug() << "Sending 'new_game' to site";
        if (!_bServiceTests)_pWebsockets->processWebsocketMsg("new_game");
    }
}

void Chess::TestMove(QString QStrMsgFromWebsockets)
{
    _pChessboard->findBoardPos(QStrMsgFromWebsockets); //oblicz wszystkie pozycje bierek

    //warunki w testach się znoszą, więc nie trzeba sprawdzać czy wykonają się oba testy.
    if(this->testPromotion()) return; //jeżeli możemy mieć doczynienia z promocją, to sprawdź tą opcję i przerwij jeśli test się powiódł.
    else if (this->testEnpassant()) return; //jeżeli możemy mieć doczynienia z enpassant, to sprawdź tą opcję i przerwij jeśli test się powiódł.
    else MovePiece(QStrMsgFromWebsockets); //jeżeli nie mamy doczynienia ze specjalnymi ruchami, to wykonuj zwykły ruch, roszadę lub bicie.
}

void Chess::MovePiece(QString QStrMsgFromWebsockets) // rządzanie ruchu- przemieszczenie bierki.
{ //TODO: mylne jest wrażenie że ta funckja już wykonuje ruch bierką
    //do tych ruchów zaliczają się: zwykły ruch, bicie, roszada.
    _pWebsockets->addTextToWebsocketConsole("Sending normal move to tcp: " + QStrMsgFromWebsockets + "\n");
    qDebug() << "Sending normal move to tcp: " << QStrMsgFromWebsockets;
    _pTCPMsgs->queueMsgs(QStrMsgFromWebsockets); //zapytaj się tcp o poprawność prośby o ruch
}

void Chess::Promote(QString QStrMsgFromWs)
{
    _pChessboard->bPromotionConfirmed = true; //w odpowiedzi na chenard ma się wykonać ruch typu...
    //...promocja, by sprawdzić czy nie ma dodatkowo bicia
    _pTCPMsgs->queueMsgs("move " + _pChessboard->QsFuturePromote + QStrMsgFromWs.mid(11,1)); //scal żądanie o ruch z żądanym typem promocji
    //dopóki fizycznie nie podmieniam pionków na nowe figury w promocji, to ruch może się odbywać jako normalne przemieszczenie
    qDebug() << "Sent to TCP: move " << _pChessboard->QsFuturePromote << QStrMsgFromWs.mid(11,1);
}

void Chess::TestOk()
{
    if(_pChessboard->bTestPromotion) //jeżeli to test na promocję wg tcp się powiódł
    {
        qDebug() << "Sending 'promote_to_what' to websockets";
        _pWebsockets->processWebsocketMsg("promote_to_what"); //to trzeba jeszcze zapytać się na WWW na co ma być ta promocja.
    }
    else if (_pChessboard->bTestEnpassant)
        this->enpassantMovingSequence(); //jeżeli test na enpassant się powiódł, to rozpocznij ten ruch
    else
    {
        qDebug() << "Error: Chess::TestOk: None test is true.";
        emit this->addTextToDobotConsole("Error: Chess::TestOk: None test is true.\n");
    }
}

void Chess::BadMove(QString QsMsgFromChenardTcp)
{
    QsMsgFromChenardTcp = QsMsgFromChenardTcp.mid(9);
    qDebug() << "Sending 'BAD_MOVE...' to websockets";
    _pWebsockets->processWebsocketMsg("BAD_MOVE " + QsMsgFromChenardTcp.simplified());
}

void Chess::GameInProgress() //gra w toku
{
    //podaj na stronę info o tym że ruch został wykonany
    qDebug() << "Chess::GameInProgress(): Sending to Websockets: game_in_progress "
             << _pChessboard->QsPiecieFromTo;
    _pWebsockets->processWebsocketMsg("game_in_progress " + _pChessboard->QsPiecieFromTo);
}
void Chess::EndOfGame(QString QStrMsgFromChenardTcp)
{
    QString QsWhoWon;
    if (QStrMsgFromChenardTcp.left(3) == "1-0") QsWhoWon = "white_won";
    else if (QStrMsgFromChenardTcp.left(3) == "0-1") QsWhoWon = "black_won";
    else if (QStrMsgFromChenardTcp.left(7) == "1/2-1/2") QsWhoWon = "draw";
    qDebug() << "Sending '" << QsWhoWon << "' to websockets";
    _pWebsockets->processWebsocketMsg(QsWhoWon);
    QsWhoWon.clear();

    this->resetPiecePositions(); //przywróć wszystkie bierki do stanu początkowego
    //TODO: zablokować możliwość robienia czegokolwiek na stronie aż to się nie zakończy
}

void Chess::resetPiecePositions() //dostaliśmy komunikat "end game" albo "new game"
{
    if (!_pChessboard->compareArrays(_pChessboard->anBoard, _pChessboard->anStartBoard))
        //jeżeli bierki stoją na szachownicy nieruszone, to nie ma potrzeby ich odstawiać
    {
        qDebug() << "start Chess::resetPiecePositions()";

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
                qDebug() << "nPieceTypeOnCheckedField =" << nPieceTypeOnCheckedField;

                //nr bierki która POWINNA BYĆ na aktualnie sprawdzanym polu (1-32)
                int nStartPieceTypeOnCheckedField = _pChessboard->anStartBoard
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)];
                qDebug() << "nStartPieceTypeOnCheckedField =" << nStartPieceTypeOnCheckedField;

                //nr oryginalnego/pierwotnego pola, na którym powinna znajdować...
                //...się bierka będąca na aktualnie sprawdzanym polu (1-16, 49-64)
                int nPieceOrigPos;
                if (nPieceTypeOnCheckedField == 0) nPieceOrigPos = 0;
                else if (nPieceTypeOnCheckedField > 16)
                    nPieceOrigPos = nPieceTypeOnCheckedField + 32; //dla czarnych bierek (17-32) są to pola 49-64
                else nPieceOrigPos = nPieceTypeOnCheckedField; //dla białych bierek pole oryginalne/pierwotne = nr bierki
                qDebug() << "nPieceOrigPos =" << nPieceOrigPos;

                //nr bierki na zbitym polu odpowiadającym polu startowemu bierki na...
                //...polu aktualnie sprawdzanym (1-32)
                int nRemovedFieldPieceType;
                if (nStartPieceTypeOnCheckedField > 0) //nie można podawać zera do fieldNrToFieldPos()
                {
                    nRemovedFieldPieceType = _pChessboard->anRemoved
                            [_pChessboard->fieldNrToFieldPos(nStartPieceTypeOnCheckedField, ROW)]
                            [_pChessboard->fieldNrToFieldPos(nStartPieceTypeOnCheckedField, COLUMN)];
                }
                else nStartPieceTypeOnCheckedField = 0;
                qDebug() << "nRemovedFieldPieceType=" << nRemovedFieldPieceType;

                if (nPieceTypeOnCheckedField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1  //...a coś tam powinno być
                            || _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) >= 6)) // (kolumny <= 1 to białe, a >= 6 to czarne)
                {
                    qDebug() << "Chess::resetPiecePositions(): checked field is empty.";
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(nRemovedFieldPieceType > 0) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        qDebug() << "Chess::resetPiecePositions(): put piece back on its empty checked field from removed area";
                        this->pieceMovingSequence('s', //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, COLUMN),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy
                    {
                        qDebug() << "Chess::resetPiecePositions(): put piece back on its empty checked field from chessboard";
                        for (int nField = 1; nField <= 64; nField++) //przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nStartPieceTypeOnCheckedField == _pChessboard->anBoard
                                    [_pChessboard->fieldNrToFieldPos(nField, ROW)] //jeżeli na tym wertowanym polu...
                                    [_pChessboard->fieldNrToFieldPos(nField, COLUMN)]  //...szachownicy jest bierka której szukamy
                                    && nCurentlyCheckedField != nField) //ale nie może to być oryginalna pozycja bierki (musi być...
                                //...na obcym polu startowym)
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
                        qDebug() << "Chess::resetPiecePositions(): put piece from checked field to its own empty start field";

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
                            qDebug() << "Chess::resetPiecePositions(): 2 pieces on each other start field. Remove 2nd, move 1st, restore 2nd.";

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
                qDebug() << "Chess::resetPiecePositions(): pieces aren't on start positions. go through loop again";

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
            else qDebug() << "Chess::resetPiecePositions(): pieces are on start positions. end of loop";
        }
         //wertuj w kółko planszę przestawiając bierki, dopóki
        //...szachownica nie wróci do stanu pierwotnego/startowego.
        while (!bArraysEqual); //wykonuj pętlę dopóki warunek jest prawdziwy

        qDebug() << "End of: Chess::resetPiecePositions()";
    }
}

#include "chess_resets.h"

ChessResets::ChessResets(Chess *pChess)
{
    _pChess = pChess;
    _pClients = _pChess->getClientsPointer();
    _pTimers->getTimersPointer();
    _pStatus->getStatusPointer();
}

void ChessResets::restartGame(END_TYPE WhoWon, Client* PlayerToClear = nullptr)
{
    //info
    QString QStrPlayer = "";
    if (PlayerToClear != nullptr)
        QStrPlayer = _pClients->getClientName(PlayerToClear) + ":";
    qDebug() << "Chess::restartGame():" << QStrPlayer << endTypeAsQstr(WhoWon);

    //reset data
    _pClients->resetPlayersStartConfirmInfo();
    _pTimers->resetGameTimers();
    _pStatus->resetStatusData();

    this->changePlayersOnChairs(WhoWon, PlayerToClear);
    this->sendEndGameMsgToAllClients(WhoWon);

    //todo: wygląda na to że funkcja resetu załącza się jeszcze zanim odpowiedź poleci na...
    //...stronę, przez co trzeba czekać aż resetowanie się zakończy zanim gracze się dowiedzą...
    //...że nastąpił koniec gry
    //todo: ogarnąć to że raz wysyłam info do WS poprzez wskaźnik do nich, a raz z tej klasy
    //future: info reseting powinno na stronie wyskakiwać zawsze, jak napotkamy koniec gry
    _pChess->sendDataToClient("reseting");

    this->resetPiecePositions();
}

void ChessResets::changePlayersOnChairs(END_TYPE WhoWon, Client* PlayerToClear)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
    case ET_TIMEOUT_GAME:
        if (PlayerToClear != nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player must be"
                        " null if not disconnected";
            return;
        }
        _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        break;
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        if (PlayerToClear == nullptr)
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): player can't be"
                        " null if diconnected";
            return;
        }
        if (_pClients->getClientType(PlayerToClear) == PT_WHITE)
            _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_WHITE);
        else if (_pClients->getClientType(PlayerToClear) == PT_BLACK)
            _pClients->cleanChairAndPutThereNextQueuedClientIfExist(PT_BLACK);
        else
        {
            qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): wrong player type:"
                     << WhoWon;
            return;
        }
        break;
    default:
        qDebug() << "ERROR: ChessResets::changePlayersOnChairs(): unknown ETWhoWon val="
                 << WhoWon;
        return;
    }
}

void ChessResets::sendEndGameMsgToAllClients(END_TYPE WhoWon)
{
    switch(WhoWon)
    {
    case ET_WHIE_WON:
    case ET_BLACK_WON:
    case ET_DRAW:
        //future: jak wysyłam table data, to nie ma potrzeby wysyłać "nt"
        //future: na przyszłość komunikat o ostatnim ruchu można wyjebać, jako że informacje...
        //...o ruchach będą wyciągane z "history"
        QString QStrMove = _pChess->getMovementsPointer()->getMove().asQStr();
        _pChess->sendDataToClient("moveOk " + QStrMove + " nt " + endTypeAsQstr(WhoWon) +
                                  " " + _pChess->getTableData());
        break;
    case ET_TIMEOUT_GAME:
    case ET_GIVE_UP:
    case ET_SOCKET_LOST:
        _pChess->sendDataToClient(endTypeAsQstr(WhoWon) +
                     playerTypeAsQStr(PlayerToClear->type) + " " + _pChess->getTableData());
        break;
    default:
        qDebug() << "ERROR: ChessResets::sendEndGameMsgToAllClients: unknown WhoWon val="
                 << WhoWon;
        return;
    }
}

//todo: cała do zmiany
void ChessResets::resetPiecePositions()
{
    if (!this->compareArrays(_pChessboard->m_asBoardMain, _pChessboard->m_anBoardStart))
        //jeżeli bierki stoją na szachownicy nieruszone, to nie ma potrzeby ich odstawiać
    {
        bool  bArraysEqual; //warunek wyjścia z poniższej pętli do..for
        bool isChessboardInvariable; //do sprawdzania czy pętla do..for się nie zacieła

        do //wertuj w kółko planszę przestawiając bierki, dopóki...
            //...szachownica nie wróci do stanu pierwotnego/startowego.
        {
            //aktualnie sprawdzane pole szachownicy
            for (int nCheckingField = 1; nCheckingField <= 64; nCheckingField++)
            {
                int nPieceNrOnCheckingField = _pChessboard->getPieceOnBoardAsNr(B_MAIN, nCheckingField);
                int nPieceStartPosOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_START, nCheckingField);
                short sCheckingFieldsStartingPieceNr = this->findInitialFieldOfGivenPiece(nPieceNrOnCheckingField);

                bool bIsStartPieceOnRemovedFieldOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_REMOVED, nPieceStartPosOfCheckingField);

                if (nPieceNrOnCheckingField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit <= D_2  //...a coś tam powinno być
                            || _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit >= D_7))

                {
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(bIsStartPieceOnRemovedFieldOfCheckingField) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        short sStartPieceOnRemovedFieldOfCheckingField = _pChessboard->getPieceOnBoardAsNr(B_REMOVED, nPieceStartPosOfCheckingField);
                        this->listMovesForDobot(ST_RESTORE, //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                _pChessboard->fieldNrToPosOnBoard(sStartPieceOnRemovedFieldOfCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(sStartPieceOnRemovedFieldOfCheckingField).Digit,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit);
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy to ...
                    {
                        for (int nField = 1; nField <= 64; nField++) //...przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nPieceStartPosOfCheckingField == _pChessboard->getPieceOnBoardAsNr(B_MAIN, nField)
                                    && nCheckingField != nField)
                                //jeżeli na tym wertowanym poluszachownicy jest bierka której szukamy ale nie może to być
                                // oryginalna pozycja bierki (musi być na obcym polu startowym)
                            { //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                                this->listMovesForDobot(ST_REGULAR,
                                                        _pChessboard->fieldNrToPosOnBoard(nField).Letter,
                                                        _pChessboard->fieldNrToPosOnBoard(nField).Digit,
                                                        _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                        _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit);
                            }
                        }
                    }
                }
                //natomiast jeżeli na sprawdzanym polu jest bierka, która nie znajdowała się tam podczas startu gry...
                //...to trzeba sprawdzić czy oryginalne pole tej bierki nie jest zajęte i tam ją odstawić
                else if (nPieceNrOnCheckingField != 0 && //jeżeli jakaś bierka na pewno tam jest
                         nPieceStartPosOfCheckingField != nPieceNrOnCheckingField) // i nie to bierka która jest tam orygnalnie
                {
                    short sPieceNrOnStartPosOfPieceOnCheckingField = _pChessboard->getPieceOnBoardAsNr(B_MAIN, sCheckingFieldsStartingPieceNr);

                    if (sPieceNrOnStartPosOfPieceOnCheckingField == 0) //czy startowe pole bierki jest puste, by tam można było ją odstawić
                    {
                        this->listMovesForDobot(ST_REGULAR,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit,
                                                _pChessboard->fieldNrToPosOnBoard(sCheckingFieldsStartingPieceNr).Letter,
                                                _pChessboard->fieldNrToPosOnBoard(sCheckingFieldsStartingPieceNr).Digit);
                    }
                    else //jeżeli startowe pole bierki na polu sprawdzanym jest zajęte przez inną bierkę
                    {
                        //pole startowe bierki która jest na polu startowym bierki aktualnie sprawdzanej
                        int nStartNrOf2ndPieceOn1stPieceStartPos = _pChessboard->getPieceOnBoardAsNr(B_START, sPieceNrOnStartPosOfPieceOnCheckingField);

                        if (nStartNrOf2ndPieceOn1stPieceStartPos == nCheckingField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            //todo: zrobić funkcję z 1 argumentem która wyczai łapaną bierkę i odstawi ją na swoje pole
                            this->listMovesForDobot(ST_REMOVING,
                                                    _pChessboard->fieldNrToPosOnBoard(nCheckingField).Letter,
                                                    _pChessboard->fieldNrToPosOnBoard(nCheckingField).Digit,
                                                    _pChessboard->fieldNrToPosOnBoard(nPieceNrOnCheckingField).Letter,
                                                    _pChessboard->fieldNrToPosOnBoard(nPieceNrOnCheckingField).Digit);
                        }
                        else qDebug() << "Chess::resetPiecePositions(): nStartNrOf2ndPieceOn1stPieceStartPos:" <<
                                         nStartNrOf2ndPieceOn1stPieceStartPos << "!= nCheckingField:" << nCheckingField;
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
            bArraysEqual = this->compareArrays(_pChessboard->m_asBoardMain,
                                                       _pChessboard->m_anBoardStart);

            if (!bArraysEqual) //jeżeli plansza jest w stanie startowym, to można nie robić reszty...
                //...poleceń i wyskoczyć z pętli
            {
                //sprawdzanie czy ustawienie bierek zmieniło się od ostatniego przejścia przez pętlę...
                //...przestawiającą bierki. Jeżeli jest różnica od ostatniego razu, to zapamiętaj że...
                //...tak jest. Pozwoli to sprawdzać czy pętla do..for nie zacięła się, a co za tym...
                //...idzie- spradzane jest czy odstawianie bierek na planszę się nie zacieło.
                isChessboardInvariable = this->compareArrays(_pChessboard->m_asBoardMain,
                                                                     _pChessboard->m_asBoardTemp);

                //jeżeli aktualne przewertowane planszy w poszukiwaniu bierek do odstawienia...
                //...spowodowało przemieszczenie się jakiejś bierki, to zapamiętaj rozmieszczenie...
                //...bierek na szachownicy do sprawdzania tego warunku w kolejnym przejściu pętli.
                if (!isChessboardInvariable)
                {
                    memcpy(_pChessboard->m_asBoardTemp, _pChessboard->m_asBoardMain,
                           sizeof(_pChessboard->m_asBoardMain));
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
    qDebug() << "Chess::resetPiecePositions(): imaginary start board and real board are equal";

    this->resetBoardCompleted();
}

void ChessResets::resetOdNowa()
{
    if (!this->isPieceSetOnStartFields())
    {
        do
        {
            for (short sField=1; sField>=64; ++sField)
            {
                Field* pField = _pChess->getBoardMainPointer()->getField(sField);
                if (pField->getStartPieceNrOnField() != pField->getPieceNrOnField())
                {
                    if (pField->getPieceNrOnField() == 0)
                    {
                        Piece* missingPiece = _pChess->getPiece(pField->getStartPieceNrOnField());
                        short sMissingPieceActualFieldNr = searchForMissingPieceActualFieldOnMainBoad(missingPiece);
                        if (sMissingPieceActualFieldNr > 0)
                        {
                            _pChess->movePieceWithManipulator(_pChess->getBoardMainPointer(),
                                                              Field::Pos(sMissingPieceActualFieldNr), VM_GRAB);
                            _pChess->movePieceWithManipulator(_pChess->getBoardMainPointer(),
                                                              Field::Pos(pField->getNr()), VM_GRAB);
                        }
                        else
                        {
                            //todo: jeżeli pole jest puste, a bierki nie ma na głównej planszy, to przywróć ja z pól zbitych
                        }
                    }
                    else //todo: jeżeli na polu jest jakaś bierka, ale nie jest to bierka startowa...
                    {

                    }
                }
            }
        }
        while (this->isPieceSetOnStartFields());
    }

    qDebug() << "All pieces are on their start fields"; //todo: wstawić do funkcji poniżej
    this->resetBoardCompleted(); //todo: not this
}

short ChessResets::searchForMissingPieceActualFieldOnMainBoad(Piece* piece)
{
    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = _pChess->getBoardMainPointer()->getField(sField);
        if (pField->getPieceNrOnField() == piece->getNr())
            return sField;
    }

    return 0;
}

bool ChessResets::isPieceSetOnStartFields()
{
    for (short sField=1; sField>=64; ++sField)
    {
        Field* pField = _pChess->getBoardMainPointer()->getField(sField);
        if (pField->getStartPieceNrOnField() != pField->getPieceNrOnField())
            return false;
    }

    //so if all pieces are on their start fields, then:
    return true;
}

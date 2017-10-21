#include "chess.h"


Chess::Chess() {} //czysto wirtualne klasy muszą mieć pusty konstruktor

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;
}

//todo: tu musi być szablon by móc używać różnych parametrów wejściowych, bo funkcja jest za duża by ją powtarzać
void Chess::listMovesForDobot(SEQUENCE_TYPE Type, LETTER pieceFromLetter, DIGIT pieceFromDigit,
                              LETTER pieceToLetter, DIGIT pieceToDigit)
{
    //jeżeli nie podano skąd i/lub dokąd ramię ma się przemieścić, to jest to ruch ramienia z aktualnego pola szachownicy nad którym ono wisi
    if (pieceFromLetter == L_X) pieceFromLetter = static_cast<LETTER>(_pChessboard->PieceFrom.Letter);
    if (pieceFromDigit == D_X) pieceFromDigit = static_cast<DIGIT>(_pChessboard->PieceFrom.Digit);
    if (pieceToLetter == L_X) pieceToLetter = static_cast<LETTER>(_pChessboard->PieceTo.Letter);
    if (pieceToDigit == D_X) pieceToDigit = static_cast<DIGIT>(_pChessboard->PieceTo.Digit);


    if (Type == ST_REMOVING) //TODO: jak już ten typ ruchu zawiera inny niż normalny typ ruchu, to znormalizować to jakoś
    {
        pieceFromLetter = pieceToLetter; //przy usuwaniu bierka "from" to ta...
        pieceFromDigit = pieceToDigit; //...która w każdym innym ruchu jest jako "to"
    }

    if (Type == ST_RESTORE && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
    {
        qDebug() << "check1 goToSafeRemovedField";
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type); //TODO: pieceToDigit nie jest poprawną daną (przy...
    //...zbijaniu czarnego z b7 (głębsza linia zbitych) ramię najpierw poszło nad białe pole zbite, a potem nad...
    //...czarne, by chyba znowu próbować wyjśc przez białe)
    }

    //todo: przesunąć wyświetlanie wszystkich komunikatów do czasu rzeczywistego

    _pDobot->gripperState(DM_OPEN, Type);
    _pDobot->pieceFromTo(DM_FROM, pieceFromLetter, pieceFromDigit, Type);
    _pDobot->armUpDown(DM_DOWN, DM_FROM, Type);
    _pDobot->gripperState(DM_CLOSE, Type);
    _pDobot->wait(400, Type);
    _pDobot->armUpDown(DM_UP, DM_FROM, Type);
    _pChessboard->pieceStateChanged(DM_FROM, pieceFromLetter, pieceFromDigit, Type);

    if ((Type == ST_REMOVING || Type == ST_RESTORE) && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
    {
        qDebug() << "check2 goToSafeRemovedField";
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type);
    }

    _pDobot->pieceFromTo(DM_TO, pieceToLetter, pieceToDigit, Type);
    _pDobot->armUpDown(DM_DOWN, DM_TO, Type);
    _pDobot->gripperState(DM_OPEN, Type);
    _pDobot->wait(400, Type);
    _pDobot->armUpDown(DM_UP, DM_TO, Type);
    _pChessboard->pieceStateChanged(DM_TO, pieceToLetter, pieceToDigit, Type);

    if (Type == ST_REMOVING && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
    {
        qDebug() << "check3 goToSafeRemovedField";
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type);
    }
    else if (Type != ST_REMOVING) _pDobot->setRetreatIndex(_pDobot->getCoreQueuedCmdIndex());
}

void Chess::goToSafeRemovedField(DIGIT digitTo, SEQUENCE_TYPE sequence)
{
    qDebug() << "goToSafeRemovedField. digitTo<0,7>=" << digitTo;
    _pDobot->writeMoveTypeInConsole(DM_INDIRECT, sequence);

    int nRemPieceNr;
    DIGIT remPieceDestDigit;
    //powrót do bezpiecznej pozycji na zbitych może odbyć się po odłożeniu zbitej bierki, a wtedy...
    //...nie ma już jej w gripperze by sprawdzić jej pole
    if (sequence == ST_REMOVING && _pChessboard->nGripperPiece != 0)
    {
        nRemPieceNr = _pChessboard->nGripperPiece;
        qDebug() << "fieldNrToFieldLinesPos1";
        remPieceDestDigit = _pChessboard->fieldNrToFieldLinesPos(nRemPieceNr).Digit;
    }
    else if (sequence == ST_RESTORE || (sequence == ST_REMOVING && _pChessboard->nGripperPiece == 0))
        //dla restore() pozycja pola na obszarze usuniętych musi być podana wprost do pieceFromTo()
    {
        //dla ST_RESTORE wartość nGripperPiece nie zadziała, bo nie ma jeszcze nic w chwytaku
        remPieceDestDigit = digitTo;
    }

    DIGIT indirectFieldDigit;
    if (remPieceDestDigit == D_1 || remPieceDestDigit == D_2) indirectFieldDigit = D_2;
    else if (remPieceDestDigit == D_3 || remPieceDestDigit == D_4) indirectFieldDigit = D_3;
    else qDebug() << "ERROR: unknown remPieceDestDigit value =" << remPieceDestDigit;

    qDebug() << "goToSafeRemovedField. indirectFieldDigit=" << indirectFieldDigit;

    float fIndirect_x = _pChessboard->m_adRemovedPiecesPositions_x[L_D][indirectFieldDigit];
    float fIndirect_y = _pChessboard->m_adRemovedPiecesPositions_y[L_D][indirectFieldDigit];
    float fIndirect_z = _pChessboard->m_adRemovedPiecesPositions_z[L_D][indirectFieldDigit];

    _pDobot->addCmdToList(DM_TO_POINT, ST_REMOVING, fIndirect_x, fIndirect_y, fIndirect_z +
                          _pChessboard->getMaxPieceHeight(), ACTUAL_POS); //TODO: getMaxRemovedPieceHeight?
}

void Chess::legalOk(QString msg) //todo: nazwy tych funkcji 'ok' nie mówią co robią
{
    QStringList legalMoves = msg.split(QRegExp("\\s"));
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove "ok"
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove np. "20"

    if (!legalMoves.isEmpty())
    {
        QString QStrLastLegalMove = legalMoves.last();
        QStrLastLegalMove = QStrLastLegalMove.simplified();
        QStrLastLegalMove = QStrLastLegalMove.replace( " ", "" ); //remove np. "\n"
        legalMoves.last() = QStrLastLegalMove;
    }
    _pChessboard->setLegalMoves(legalMoves);
}

void Chess::historyOk(QString msg)
{
    QStringList historyMoves = msg.split(QRegExp("\\s"));
    if (!historyMoves.isEmpty()) historyMoves.removeFirst(); //remove "ok"
    if (!historyMoves.isEmpty()) historyMoves.removeFirst(); //remove np. "20"

    if (!historyMoves.isEmpty()) //whipe CR, LF, spacebars etc
    {
        QString QStrLastHistoryMove = historyMoves.last();
        QStrLastHistoryMove = QStrLastHistoryMove.simplified();
        QStrLastHistoryMove = QStrLastHistoryMove.replace( " ", "" ); //remove np. "\n"
        historyMoves.last() = QStrLastHistoryMove;
    }

    _pChessboard->setHistoryMoves(historyMoves);
}

void Chess::wrongTcpAnswer(QString msgType, QString respond)
{
    qDebug() << "ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void Chess::castlingMovingSequence()
{
    this->listMovesForDobot(ST_CASTLING_KING); //wykonaj przemieszczenie królem
    _pChessboard->castlingFindRookToMove(); //podmień pozycje ruszonego króla na pozycję wieży
    this->listMovesForDobot(ST_CASTLING_ROOK); //wykonaj przemieszczenie wieżą
}

void Chess::enpassantMovingSequence()
{
    //wykonaj normalny ruch. enpassant to jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.
    this->listMovesForDobot(ST_ENPASSANT);

    DIGIT tempDigitPos = _pChessboard->PieceTo.Digit; //w razie czego zapaiętaj oryginalną wartość pola (cyfry) bijącego
    if (_pChessboard->getWhoseTurn() == WHITE_TURN)
    {
        //pozycja zbijanego czarnego pionka przez pionka białego w jego turze (białego)
        _pChessboard->PieceTo.Digit = static_cast<DIGIT>((int)_pChessboard->PieceTo.Digit - 1); //todo: sprawdzić czy dziala
    }
    else if (_pChessboard->getWhoseTurn() == BLACK_TURN)
    {
        //pozycja zbijanego białego pionka przez pionka czarnego w jego turze (czarnego)
        _pChessboard->PieceTo.Digit = static_cast<DIGIT>((int)_pChessboard->PieceTo.Digit + 1);
    }
    else
    {
        emit _pDobot->addTextToConsole("Error03!: Wrong turn in enpassant statement: "
                                       + (QString)_pChessboard->getWhoseTurn() + "/n", LOG_DOBOT);
        qDebug() << "Error03!: Chess::enpassantMovingSequence(): Unknown turn type: "
                 << _pChessboard->getWhoseTurn() << "/n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    this->listMovesForDobot(ST_REMOVING); //usuń pionka bitego ze  zmienioną pozycją.
    _pChessboard->PieceTo.Digit = tempDigitPos; //wróć prewencyjnie w pamięci do normalnej pozycji
}

//todo: reset i jego funkcje zrobić jako klasę-organ
short Chess::findInitialPieceNrOnGivenField(short sField)
{
    if (sField < 1 || sField > 64)
    {
        qDebug() << "ERROR: Chess::findInitialPieceNrOnGivenField: field out of scope <1,64>:" << sField;
        return 0;
    }

    short sInitPieceNr;

    if (sField <= 16)
        sInitPieceNr = sField; //dla pól białych bierek pole oryginalne = nr bierki
    else if (sField > 48)
        sInitPieceNr = sField - 32; //dla pól czarnych bierek (49-64) są to bierki 17-32
    else sInitPieceNr = 0; //na środkowych rzędach pól nie ma startowo bierek

    return sInitPieceNr;
}


short Chess::findInitialFieldOfGivenPiece(short sPiece)
{
    if (sPiece < 0 || sPiece > 32)
    {
        qDebug() << "ERROR: Chess::findInitialFieldOfGivenPiece: piece out of scope <0,32>:" << sPiece;
        return 0;
    }

    short sInitFieldNr;

    if (sPiece <= 16)
        sInitFieldNr = sPiece; //dla białych bierek pole oryginalne = nr bierki
    else  sInitFieldNr = sPiece + 32; //dla czarnych bierek (17-32) są to pola 49-64

    return sInitFieldNr;
}

void Chess::resetPiecePositions()
{
    if (!_pChessboard->compareArrays(_pChessboard->m_asBoardMain, _pChessboard->m_anBoardStart))
        //jeżeli bierki stoją na szachownicy nieruszone, to nie ma potrzeby ich odstawiać
    {
        qDebug() << "start Chess::resetPiecePositions()";

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

                //nr oryginalnego/pierwotnego pola, na którym powinna znajdować...
                //...się bierka będąca na aktualnie sprawdzanym polu (1-16, 49-64)
                int nPieceStartPos = this->findInitialFieldOfGivenPiece(nPieceNrOnCheckingField);
                short sPieceNrOnStartPosOfPieceOnCheckingField = _pChessboard->getPieceOnBoardAsNr(B_MAIN, nPieceStartPos);
                qDebug() << "nCheckingField =" << nCheckingField << ", nPieceNrOnCheckingField =" <<
                            nPieceNrOnCheckingField << ", nPieceStartPosOfCheckingField =" <<
                            nPieceStartPosOfCheckingField << ", nPieceStartPos =" << nPieceStartPos <<
                            ", sPieceNrOnStartPosOfPieceOnCheckingField =" << sPieceNrOnStartPosOfPieceOnCheckingField;

                //nr bierki na zbitym polu odpowiadającym polu startowemu bierki na...
                //...polu aktualnie sprawdzanym (1-32)
                int nRemovedFieldPieceNr;
                if (nPieceStartPosOfCheckingField > 0) //nie można podawać zera do mianownika (w funkcji)
                    nRemovedFieldPieceNr = _pChessboard->getPieceOnBoardAsNr(B_REMOVED, nPieceStartPosOfCheckingField);
                else nPieceStartPosOfCheckingField = 0; //todo: sprawdzić cały ten warunek

                qDebug() << "fieldNrToFieldLinesPos2&3";
                if (nPieceNrOnCheckingField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit <= D_2  //...a coś tam powinno być
                            || _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit >= D_7))

                {
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(nRemovedFieldPieceNr > 0) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        qDebug() << "check1. nRemovedFieldPieceNr =" << nRemovedFieldPieceNr <<
                                    ", nCheckingField =" << nCheckingField;
                        qDebug() << "fieldNrToFieldLinesPos4";
                        this->listMovesForDobot(ST_RESTORE, //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                _pChessboard->fieldNrToFieldLinesPos(nRemovedFieldPieceNr).Letter,
                                                _pChessboard->fieldNrToFieldLinesPos(nRemovedFieldPieceNr).Digit,
                                                _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Letter,
                                                _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit);
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
                                qDebug() << "check2. nField =" << nField <<
                                            ", nCheckingField =" << nCheckingField;
                                qDebug() << "fieldNrToFieldLinesPos5";
                                this->listMovesForDobot(ST_REGULAR,
                                                        _pChessboard->fieldNrToFieldLinesPos(nField).Letter,
                                                        _pChessboard->fieldNrToFieldLinesPos(nField).Digit,
                                                        _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Letter,
                                                        _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit);
                            }
                        }
                    }
                }
                //natomiast jeżeli na sprawdzanym polu jest bierka, która nie znajdowała się tam podczas startu gry...
                //...to trzeba sprawdzić czy oryginalne pole tej bierki nie jest zajęte i tam ją odstawić
                else if (nPieceNrOnCheckingField != 0 && //jeżeli jakaś bierka na pewno tam jest
                         nPieceStartPosOfCheckingField != nPieceNrOnCheckingField) // i nie to bierka która jest tam orygnalnie
                {
                        //TODO: TU TKWI PROBLEM: NAKIEROWANIE JEST TYLKO NA POLE, A NIE NA BIERKĘ KTÓRA TAM MOŻE STAĆ. CHYBA

                    if (sPieceNrOnStartPosOfPieceOnCheckingField == 0) //czy startowe pole bierki jest puste, by tam można było ją odstawić
                    {
                        qDebug() << "Chess::resetPiecePositions(): startowe pole bierki rozpatrywanej do odłożenia"
                                    " jest puste- sPieceNrOnStartPosOfPieceOnCheckingField == 0";
                        //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                        qDebug() << "check3. nCheckingField =" << nCheckingField <<
                                    ", sPieceNrOnStartPosOfPieceOnCheckingField =" << sPieceNrOnStartPosOfPieceOnCheckingField;
                        qDebug() << "fieldNrToFieldLinesPos6";
                        this->listMovesForDobot(ST_REGULAR,
                                                _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Letter,
                                                _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit,
                                                _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Letter,
                                                _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Digit);
                    }
                    else //jeżeli startowe pole bierki na polu sprawdzanym jest zajęte przez inną bierkę
                    {
                        qDebug() << "Chess::resetPiecePositions(): startowe pole bierki rozpatrywanej do odlozenia NIE jest puste- "
                                    "sPieceNrOnStartPosOfPieceOnCheckingField != 0. it's' =" << sPieceNrOnStartPosOfPieceOnCheckingField;

                        //pole startowe bierki która jest na polu startowym bierki aktualnie sprawdzanej
                        int nStartNrOf2ndPieceOn1stPieceStartPos = _pChessboard->getPieceOnBoardAsNr(B_START, sPieceNrOnStartPosOfPieceOnCheckingField);
                        qDebug() << "Chess::resetPiecePositions(): nStartNrOf2ndPieceOn1stPieceStartPos =" << nStartNrOf2ndPieceOn1stPieceStartPos;

                        if (nStartNrOf2ndPieceOn1stPieceStartPos == nCheckingField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            qDebug() << "Chess::resetPiecePositions(): 2 pieces on each other start field. Remove 2nd, move 1st, restore 2nd.";

                            qDebug() << "check4. nPieceStartPos =" << nPieceStartPos <<
                                        ", sPieceNrOnStartPosOfPieceOnCheckingField =" << sPieceNrOnStartPosOfPieceOnCheckingField <<
                                        ", nCheckingField =" << nCheckingField;
                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            qDebug() << "fieldNrToFieldLinesPos7";
                            this->listMovesForDobot(ST_REMOVING,
                                                    _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Digit,
                                                    _pChessboard->fieldNrToFieldLinesPos(sPieceNrOnStartPosOfPieceOnCheckingField).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(sPieceNrOnStartPosOfPieceOnCheckingField).Digit);

                            //teraz przenieś bierkę z pola aktualnie sprawdzanego, na pole bierki dopiero co odłożonej na obszar bierek zbitych
                            qDebug() << "fieldNrToFieldLinesPos8";
                            this->listMovesForDobot(ST_REGULAR,
                                                    _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit,
                                                    _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(nPieceStartPos).Digit);

                            //na końcu bierkę odłożoną poza planszę przywróć na pole bierki wcześniej przestawionej
                            qDebug() << "fieldNrToFieldLinesPos9";
                            this->listMovesForDobot(ST_RESTORE,
                                                    _pChessboard->fieldNrToFieldLinesPos(sPieceNrOnStartPosOfPieceOnCheckingField).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(sPieceNrOnStartPosOfPieceOnCheckingField).Digit,
                                                    _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Letter,
                                                    _pChessboard->fieldNrToFieldLinesPos(nCheckingField).Digit);
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
            bArraysEqual = _pChessboard->compareArrays(_pChessboard->m_asBoardMain,
                                                       _pChessboard->m_anBoardStart);

            if (!bArraysEqual) //jeżeli plansza jest w stanie startowym, to można nie robić reszty...
                //...poleceń i wyskoczyć z pętli
            {
                qDebug() << "Chess::resetPiecePositions(): pieces aren't on start positions. go through loop again";

                //sprawdzanie czy ustawienie bierek zmieniło się od ostatniego przejścia przez pętlę...
                //...przestawiającą bierki. Jeżeli jest różnica od ostatniego razu, to zapamiętaj że...
                //...tak jest. Pozwoli to sprawdzać czy pętla do..for nie zacięła się, a co za tym...
                //...idzie- spradzane jest czy odstawianie bierek na planszę się nie zacieło.
                isChessboardInvariable = _pChessboard->compareArrays(_pChessboard->m_asBoardMain,
                                                                     _pChessboard->m_asBoardTemp);

                //jeżeli aktualne przewertowane planszy w poszukiwaniu bierek do odstawienia...
                //...spowodowało przemieszczenie się jakiejś bierki, to zapamiętaj rozmieszczenie...
                //...bierek na szachownicy do sprawdzania tego warunku w kolejnym przejściu pętli.
                if (!isChessboardInvariable)
                {
                    memcpy(_pChessboard->m_asBoardTemp, _pChessboard->m_asBoardMain,
                           sizeof(_pChessboard->m_asBoardMain)); //todo: pseudooperator m_asBoardTemp = m_asBoardMain
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
    qDebug() << "Chess::resetBoardCompleted(): imaginary start board and real board are equal";
    this->resetBoardCompleted();
}

void Chess::handleMove(QString move)
{
    qDebug() << "Chess::handleMove =" << move;
    _pChessboard->findBoardPos(move);

    _pChessboard->setMoveType(this->findMoveType(move));
    qDebug() << "move type =" << _pChessboard->getMoveType();

    switch(_pChessboard->getMoveType())
    {
    case ST_PROMOTE_TO_WHAT:
        this->PromoteToWhat(move);
        break;
    case ST_ENPASSANT:
        this->enpassantMovingSequence();
        this->MoveTcpPiece("move " + move);
        break;
    case ST_CASTLING:
        this->castlingMovingSequence();
        this->MoveTcpPiece("move " + move);
        break;
    case ST_REMOVING:
        this->listMovesForDobot(ST_REMOVING);
        this->listMovesForDobot(ST_REGULAR);
        this->MoveTcpPiece("move " + move);
        break;
    case ST_REGULAR:
        this->listMovesForDobot(ST_REGULAR); //TODO: to wygląda jakby to robił dobot wszystko, ten ruch tj.
        this->MoveTcpPiece("move " + move);
        break;
    case ST_BADMOVE:
        this->BadMove(move);
        break;
    case ST_NONE:
    default:
        qDebug() << "ERROR: Chess::handleMove: wrong MoveType :" << _pChessboard->getMoveType();
        break;
    }
}

SEQUENCE_TYPE Chess::findMoveType(QString move)
{
    if (_pChessboard->getLegalMoves().contains(move + "q")) return ST_PROMOTE_TO_WHAT;
    else if (_pChessboard->getLegalMoves().contains(move))
    {
        if (_pChessboard->isMoveEnpassant(move)) return ST_ENPASSANT;
        else if (_pChessboard->isMoveCastling(move)) return ST_CASTLING;
        else if (_pChessboard->isMoveRemoving()) return ST_REMOVING;
        else return ST_REGULAR;
    }
    else return ST_BADMOVE;
}

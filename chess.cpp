#include "chess.h"


Chess::Chess() {} //czysto wirtualne klasy muszą mieć pusty konstruktor

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;

    _bServiceTests = false;
}

void Chess::pieceMovingSequence(SEQUENCE_TYPE Type, LETTER pieceFromLetter, DIGIT pieceFromDigit,
                                LETTER pieceToLetter, DIGIT pieceToDigit)
{
    //jeżeli nie podano skąd i/lub dokąd, to jest to ruch z aktualnego pola szachownicy nad którym wisi ramie
    if (pieceFromLetter == L_X) pieceFromLetter = static_cast<LETTER>(_pChessboard->PieceFrom.Letter);
    if (pieceFromDigit == D_X) pieceFromDigit = static_cast<DIGIT>(_pChessboard->PieceFrom.Digit);
    if (pieceToLetter == L_X) pieceToLetter = static_cast<LETTER>(_pChessboard->PieceTo.Letter);
    if (pieceToDigit == D_X) pieceToDigit = static_cast<DIGIT>(_pChessboard->PieceTo.Digit);

    if (Type == REMOVING) //TODO: jak już ten typ ruchu zawiera inny niż normalny typ ruchu, to znormalizować to jakoś
    {
        pieceFromLetter = pieceToLetter; //przy usuwaniu bierka "from" to ta...
        pieceFromDigit = pieceToDigit; //...która w każdym innym ruchu jest jako "to"
    }

    if (Type == RESTORE && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type); //TODO: pieceToDigit nie jest poprawną daną (przy...
        //...zbijaniu czarnego z b7 (głębsza linia zbitych) ramię najpierw poszło nad białe pole zbite, a potem nad...
        //...czarne, by chyba znowu próbować wyjśc przez białe)

    //todo: przesunąć wyświetlanie wszystkich komunikatów do czasu rzeczywistego

    _pDobot->gripperState(OPEN, Type);
    _pDobot->pieceFromTo(FROM, pieceFromLetter, pieceFromDigit, Type);
    _pDobot->armUpDown(DOWN, FROM, Type);
    _pDobot->gripperState(CLOSE, Type);
    _pDobot->wait(400, Type);
    _pDobot->armUpDown(UP, FROM, Type);
    _pChessboard->pieceStateChanged(FROM, pieceFromLetter, pieceFromDigit, Type);

    if ((Type == REMOVING || Type == RESTORE) && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type);

    _pDobot->pieceFromTo(TO, pieceToLetter, pieceToDigit, Type);
    _pDobot->armUpDown(DOWN, TO, Type);
    _pDobot->gripperState(OPEN, Type);
    _pDobot->wait(400, Type);
    _pDobot->armUpDown(UP, TO, Type);
    _pChessboard->pieceStateChanged(TO, pieceToLetter, pieceToDigit, Type);

    if (Type == REMOVING && (pieceToLetter == L_A || pieceToLetter == L_B || pieceToLetter == L_C))
        this->goToSafeRemovedField((DIGIT)pieceToDigit, Type);
    else if (Type != REMOVING) _pDobot->setRetreatIndex(_pDobot->getCoreQueuedCmdIndex());
}

void Chess::goToSafeRemovedField(DIGIT digitTo, SEQUENCE_TYPE sequence)
{
    qDebug() << "goToSafeRemovedField. digitTo=" << digitTo;
    _pDobot->writeMoveTypeInConsole(INDIRECT, sequence);

    int nRemPieceType;
    DIGIT remPieceDestDigit;
    if (sequence == REMOVING) //TODO: kod skopiowany praktycznie z Dobot::pieceFromTo. nie wiem czy zadziala
    {
        nRemPieceType = _pChessboard->nGripperPiece; //bierka z planszy w chwytaku do usunięcia
        remPieceDestDigit = static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nRemPieceType, COLUMN));
    }
    else if (sequence == RESTORE) //dla restore() pozycja pola na obszarze usuniętych musi...
        //...być podana wprost do pieceFromTo()
    {
        //dla RESTORE wartość nGripperPiece nie zadziała, bo nie ma jeszcze nic w chwytaku
        remPieceDestDigit = digitTo;
    }
    DIGIT indirectFieldDigit;
    if (remPieceDestDigit == D_1 || remPieceDestDigit == D_2) indirectFieldDigit = D_2;
    else if (remPieceDestDigit == D_3 || remPieceDestDigit == D_4) indirectFieldDigit = D_3;
    else qDebug() << "ERROR: unknown pieceToDigit value =" << digitTo;

    qDebug() << "goToSafeRemovedField. indirectFieldDigit=" << indirectFieldDigit;

    float fIndirect_x = _pChessboard->adRemovedPiecesPositions_x[L_D][indirectFieldDigit];
    float fIndirect_y = _pChessboard->adRemovedPiecesPositions_y[L_D][indirectFieldDigit];
    float fIndirect_z = _pChessboard->adRemovedPiecesPositions_z[L_D][indirectFieldDigit];

    _pDobot->addCmdToList(TO_POINT, REMOVING, fIndirect_x, fIndirect_y, fIndirect_z +
                          _pChessboard->getMaxPieceHeight(), ACTUAL_POS); //TODO: getMaxRemovedPieceHeight?
}

void Chess::legalOk(QString msg)
{
    QStringList legalMoves = msg.split(QRegExp("\\s"));
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove "ok"
    if (!legalMoves.isEmpty()) legalMoves.removeFirst(); //remove np. "20"
    //
    if (!legalMoves.isEmpty())
    {
        QString QStrLastLegalMove = legalMoves.last();
        QStrLastLegalMove = QStrLastLegalMove.simplified();
        QStrLastLegalMove = QStrLastLegalMove.replace( " ", "" ); //remove np. "\n"
        legalMoves.last() = QStrLastLegalMove;
    }
    _pChessboard->setLegalMoves(legalMoves);
}

void Chess::wrongTcpAnswer(QString msgType, QString respond)
{
    qDebug() << "ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void Chess::castlingMovingSequence()
{
    this->pieceMovingSequence(CASTLING_KING); //wykonaj przemieszczenie królem
    _pChessboard->castlingFindRookToMove(); //podmień pozycje ruszonego króla na pozycję wieży
    this->pieceMovingSequence(CASTLING_ROOK); //wykonaj przemieszczenie wieżą
}

void Chess::enpassantMovingSequence()
{
    //wykonaj normalny ruch. enpassant to jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.
    this->pieceMovingSequence(ENPASSANT);

    DIGIT tempDigitPos = _pChessboard->PieceTo.Digit; //w razie czego zapaiętaj oryginalną wartość pola (cyfry) bijącego
    if (_pChessboard->getWhoseTurn() == WHITE_TURN)
    {
        //pozycja zbijanego czarnego pionka przez pionka białego w jego turze (białego)
        _pChessboard->PieceTo.Digit = static_cast<DIGIT>((int)_pChessboard->PieceTo.Digit - 1); //todo: sprawdzi czy dziala
    }
    else if (_pChessboard->getWhoseTurn() == BLACK_TURN)
    {
        //pozycja zbijanego białego pionka przez pionka czarnego w jego turze (czarnego)
        _pChessboard->PieceTo.Digit = static_cast<DIGIT>((int)_pChessboard->PieceTo.Digit + 1);
    }
    else
    {
        emit _pDobot->addTextToConsole("Error03!: Wrong turn in enpassant statement: "
                                            + (QString)_pChessboard->getWhoseTurn() + "/n", DOBOT);
        qDebug() << "Error03!: Chess::enpassantMovingSequence(): Unknown turn type: "
                 << _pChessboard->getWhoseTurn() << "/n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    this->pieceMovingSequence(REMOVING); //usuń pionka bitego ze  zmienioną pozycją.
    _pChessboard->PieceTo.Digit = tempDigitPos; //wróć prewencyjnie w pamięci do normalnej pozycji
}

void Chess::resetPiecePositions()
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
            //aktualnie sprawdzane pole szachownicy
            for (int nCurentlyCheckedField = 1; nCurentlyCheckedField <= 64; nCurentlyCheckedField++)
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
                else if (nPieceTypeOnCheckedField > 16)
                    nPieceOrigPos = nPieceTypeOnCheckedField + 32; //dla czarnych bierek (17-32) są to pola 49-64
                else nPieceOrigPos = nPieceTypeOnCheckedField; //dla białych bierek pole oryginalne/pierwotne = nr bierki

                //nr bierki na zbitym polu odpowiadającym polu startowemu bierki na...
                //...polu aktualnie sprawdzanym (1-32)
                int nRemovedFieldPieceType;
                if (nStartPieceTypeOnCheckedField > 0) //nie można podawać zera do fieldNrToFieldPos(), tj. mianownika
                {
                    nRemovedFieldPieceType = _pChessboard->anRemoved
                            [_pChessboard->fieldNrToFieldPos(nStartPieceTypeOnCheckedField, ROW)]
                            [_pChessboard->fieldNrToFieldPos(nStartPieceTypeOnCheckedField, COLUMN)];
                }
                else nStartPieceTypeOnCheckedField = 0;

                if (nPieceTypeOnCheckedField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1  //...a coś tam powinno być
                            //(kolumny <= 1 to białe, a >= 6 to czarne)
                            || _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) >= 6))
                {
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(nRemovedFieldPieceType > 0) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        //qDebug() << "Chess::resetPiecePositions(): put piece back on its empty checked field from removed area";
                        this->pieceMovingSequence(RESTORE, //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                  static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, ROW)),
                                                  static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, COLUMN)),
                                                  static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)),
                                                  static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)));
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy
                    {
                        for (int nField = 1; nField <= 64; nField++) //przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nStartPieceTypeOnCheckedField == _pChessboard->anBoard
                                    [_pChessboard->fieldNrToFieldPos(nField, ROW)] //jeżeli na tym wertowanym polu...
                                    [_pChessboard->fieldNrToFieldPos(nField, COLUMN)]  //...szachownicy jest bierka której szukamy
                                    && nCurentlyCheckedField != nField) //ale nie może to być oryginalna pozycja bierki (musi być...
                                //...na obcym polu startowym)
                            {
                                //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                                this->pieceMovingSequence(REGULAR,
                                                          static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nField, ROW)),
                                                          static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nField, COLUMN)),
                                                          static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)),
                                                          static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)));
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
                        //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                        this->pieceMovingSequence(REGULAR,
                                                  static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)),
                                                  static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)),
                                                  static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW)),
                                                  static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN)));
                    }
                    else //jeżeli startowe pole bierki na polu sprawdzanym jest zajęte przez inną bierkę
                    {
                        int nNrOf2ndPieceOn1stPieceOrigPos = //bierka znajdująca się na polu starowym bierki będącej na...
                                _pChessboard->anBoard[_pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW)] //...polu aktualnie sprawdzanym.
                                [_pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN)];

                        int nStartNrOf2ndPieceOn1stPieceOrigPos = _pChessboard->anStartBoard //pole startowe bierki znajdującej się na polu...
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW)] //...startowym bierki, która jest...
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN)]; //...na polu aktualnie sprawdzanym.

                        if (nStartNrOf2ndPieceOn1stPieceOrigPos == nCurentlyCheckedField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            qDebug() << "Chess::resetPiecePositions(): 2 pieces on each other start field. Remove 2nd, move 1st, restore 2nd.";

                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            this->pieceMovingSequence(REMOVING,
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN)),
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN)));

                            //teraz przenieś bierkę z pola aktualnie sprawdzanego, na pole bierki dopiero co odłożonej na obszar bierek zbitych
                            this->pieceMovingSequence(REGULAR,
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)),
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN)));

                            //na końcu bierkę odłożoną poza planszę przywróć na pole bierki wcześniej przestawionej
                            this->pieceMovingSequence(RESTORE,
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN)),
                                                      static_cast<LETTER>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)),
                                                      static_cast<DIGIT>(_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)));
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
    qDebug() << "Chess::resetBoardCompleted().";
    this->resetBoardCompleted();
}

void Chess::handleMove(QString move)
{
    qDebug() << "Chess::handleMove =" << move;
    _pChessboard->findBoardPos(move);

    _pChessboard->setMoveType(this->checkMoveType(move));
    qDebug() << "move type =" << _pChessboard->getMoveType();

    switch(_pChessboard->getMoveType())
    {
    case PROMOTE_TO_WHAT:
        this->PromoteToWhat(move);
        break;
    case ENPASSANT:
        this->enpassantMovingSequence();
        this->MoveTcpPiece("move " + move);
        break;
    case CASTLING:
        this->castlingMovingSequence();
        this->MoveTcpPiece("move " + move);
        break;
    case REMOVING:
        this->pieceMovingSequence(REMOVING);
        this->pieceMovingSequence(REGULAR);
        this->MoveTcpPiece("move " + move);
        break;
    case REGULAR:
        this->pieceMovingSequence(REGULAR); //TODO: to wygląda jakby to robił dobot wszystko, ten ruch tj.
        this->MoveTcpPiece("move " + move);
        break;
    case BADMOVE:
        this->BadMove(move);
        break;
    case NONE:
    default:
        qDebug() << "ERROR: Chess::handleMove: wrong MoveType :" << _pChessboard->getMoveType();
        break;
    }
}

SEQUENCE_TYPE Chess::checkMoveType(QString move) //TODO: check --> find ?
{
    if (_pChessboard->getLegalMoves().contains(move + "q")) return PROMOTE_TO_WHAT;
    else if (_pChessboard->getLegalMoves().contains(move))
    {
        if (_pChessboard->isMoveEnpassant(move)) return ENPASSANT;
        else if (_pChessboard->isMoveCastling(move)) return CASTLING;
        else if (_pChessboard->isMoveRemoving()) return REMOVING;
        else return REGULAR;
    }
    else return BADMOVE;
}

#include "chess.h"

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

//czysto wirtualne klasy muszą mieć pusty konstruktor
Chess::Chess() {}

Chess::Chess(Dobot *pDobot, Chessboard *pChessboard, TCPMsgs *pTCPMsgs, WebTable *pWebTable)
{
    _pTCPMsgs = pTCPMsgs;
    _pDobot = pDobot;
    _pChessboard = pChessboard;
    _pWebTable = pWebTable;

    _bServiceTests = false;
}

void Chess::pieceMovingSequence(MOVE_TYPE Type, int nPieceFromLetter, int nPieceFromDigit,
                                int nPieceToLetter, int nPieceToDigit)
{
    _pDobot->writeMoveTypeInConsole(Type);

    //jeżeli nie podano skąd i/lub dokąd, to jest to ruch z aktualnego pola szachownicy nad którym wisi ramie
    if (nPieceFromLetter == -1) nPieceFromLetter = _pChessboard->PieceFrom.Letter;
    if (nPieceFromDigit == -1) nPieceFromDigit = _pChessboard->PieceFrom.Digit;
    if (nPieceToLetter == -1) nPieceToLetter = _pChessboard->PieceTo.Letter;
    if (nPieceToDigit == -1) nPieceToDigit = _pChessboard->PieceTo.Digit;

    if (Type == REMOVING) //TODO: jak już ten typ ruchu zawiera inny niż normalny typ ruchu, to znormalizować to jakoś
    {
        nPieceFromLetter = nPieceToLetter; //przy usuwaniu bierka "from" to ta...
        nPieceFromDigit = nPieceToDigit; //...która w każdym innym ruchu jest jako "to"  
    }

    if (Type == RESTORE) _pDobot->addCmdToList(-1, false, 260, -10, 40);

    qDebug() << "-Start move sequence-";
    emit this->addTextToConsole("-Start move sequence-\n", 'd'); //TODO: nie wyswietla sie (?)

    _pDobot->gripperOpennedState(OPEN, Type);
    _pDobot->pieceFromTo(FROM, nPieceFromLetter, nPieceFromDigit, Type);
    _pDobot->armUpDown(DOWN, FROM, Type);
    _pDobot->gripperOpennedState(CLOSE, Type);
    _pDobot->wait(400);
    _pDobot->armUpDown(UP, FROM, Type);
    _pChessboard->pieceStateChanged(FROM, nPieceFromLetter, nPieceFromDigit, Type);

    if (Type == REMOVING || Type == RESTORE) _pDobot->addCmdToList(-1, false, 260, -10, 40);

    _pDobot->pieceFromTo(TO, nPieceToLetter, nPieceToDigit, Type);
    _pDobot->armUpDown(DOWN, TO, Type);
    _pDobot->gripperOpennedState(OPEN, Type);
    _pDobot->wait(400);
    _pDobot->armUpDown(UP, TO, Type);
    _pChessboard->pieceStateChanged(TO, nPieceToLetter, nPieceToDigit, Type);

    if (Type == REMOVING) _pDobot->addCmdToList(-1, false, 260, -10, 40);

    qDebug() << "-End of move sequence-";
    emit this->addTextToConsole("-End of move sequence-\n", 'd'); //TODO: nie wyswietla sie (?)
}

void Chess::wrongTcpAnswer(QString msgType, QString respond)
{
     emit this->addTextToConsole("ERROR: Chess::wrongTcpAnswer(): unknown tcpRespond = " +
                                  respond + "for tcpMsgType = " + msgType + "\n", 'c');
    qDebug() << "ERROR: IgorBot::wrongTcpAnswer(): unknown tcpRespond = " <<
                respond << "for tcpMsgType = " << msgType;
}

void Chess::castlingMovingSequence()
{
    //todo: ogarnąć jakoś nadmiar zmiennych typu castling
    this->pieceMovingSequence(CASTLING_KING); //wykonaj przemieszczenie królem
    _pChessboard->castlingFindRookToMove(); //podmień pozycje ruszonego króla na pozycję wieży
    this->pieceMovingSequence(CASTLING_ROOK); //wykonaj przemieszczenie wieżą
}

void Chess::enpassantMovingSequence()
{
    //wykonaj normalny ruch. enpassant to jedyny przypadek bicia, gdzie mogę zbić po normalnym ruchu.
    this->pieceMovingSequence(ENPASSANT);

    int nTempDigitPos = _pChessboard->PieceTo.Digit; //w razie czego zapaiętaj oryginalną wartość pola (cyfry) bijącego
    if (_pChessboard->getWhoseTurn() == WHITE_TURN) //todo
        _pChessboard->PieceTo.Digit -= 1; //pozycja zbijanego czarnego pionka przez pionka białego w jego turze (białego)
    else if (_pChessboard->getWhoseTurn() == BLACK_TURN)
        _pChessboard->PieceTo.Digit += 1; //pozycja zbijanego białego pionka przez pionka czarnego w jego turze (czarnego)
    else
    {
        emit _pDobot->addTextToConsole("Error03!: Wrong turn in enpassant statement: "
                                            + (QString)_pChessboard->getWhoseTurn() + "/n", 'd');
        qDebug() << "Error03!: Chess::enpassantMovingSequence(): Unknown turn type: "
                 << _pChessboard->getWhoseTurn() << "/n";
        return;
    }
    //wyjątkowo zbijany będzie gdzie indziej niż lądujący (w enpassant zawsze występuje bicie)
    this->pieceMovingSequence(REMOVING); //usuń pionka bitego ze  zmienioną pozycją.
    _pChessboard->PieceTo.Digit = nTempDigitPos; //wróć prewencyjnie w pamięci do normalnej pozycji
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
                //qDebug() << "nCurentlyCheckedField =" << nCurentlyCheckedField;
                /*qDebug() << "ROW =" << ROW << ", COLUMN =" << COLUMN;
                qDebug() << "_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW) ="
                         << _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW);
                qDebug() << "_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) ="
                         << _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN);
                qDebug() << "nPieceTypeOnCheckedField =" << nPieceTypeOnCheckedField;*/

                //nr bierki która POWINNA BYĆ na aktualnie sprawdzanym polu (1-32)
                int nStartPieceTypeOnCheckedField = _pChessboard->anStartBoard
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW)]
                        [_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN)];
                //qDebug() << "nStartPieceTypeOnCheckedField =" << nStartPieceTypeOnCheckedField;

                //nr oryginalnego/pierwotnego pola, na którym powinna znajdować...
                //...się bierka będąca na aktualnie sprawdzanym polu (1-16, 49-64)
                int nPieceOrigPos;
                if (nPieceTypeOnCheckedField == 0) nPieceOrigPos = 0;
                else if (nPieceTypeOnCheckedField > 16)
                    nPieceOrigPos = nPieceTypeOnCheckedField + 32; //dla czarnych bierek (17-32) są to pola 49-64
                else nPieceOrigPos = nPieceTypeOnCheckedField; //dla białych bierek pole oryginalne/pierwotne = nr bierki
                //qDebug() << "nPieceOrigPos =" << nPieceOrigPos;

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
                //qDebug() << "nRemovedFieldPieceType=" << nRemovedFieldPieceType;

                if (nPieceTypeOnCheckedField == 0 //jeżeli na sprawdzanym polu nie ma żadnej bierki...
                        && (_pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) <= 1  //...a coś tam powinno być
                            //(kolumny <= 1 to białe, a >= 6 to czarne)
                            || _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN) >= 6))
                {
                    //qDebug() << "Chess::resetPiecePositions(): checked field is empty.";
                    //sprawdź najpierw po numerze bierki czy leży ona na swoim zbitym miejscu na polu zbitych bierek
                    if(nRemovedFieldPieceType > 0) //jeżeli na polu zbitych jest bierka, która startowo stoi na aktualnie sprawdzanym polu
                    {
                        //qDebug() << "Chess::resetPiecePositions(): put piece back on its empty checked field from removed area";
                        this->pieceMovingSequence(RESTORE, //przenieś bierkę z pól zbitych bierek na oryginalne pole bierki na szachownicy
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nRemovedFieldPieceType, COLUMN),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                  _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN));
                    }
                    else //jeżeli nie ma bierki na polu zbitych bierek, to leży ona gdzieś na szachownicy
                    {
                        //qDebug() << "Chess::resetPiecePositions(): put piece back on its empty checked field from chessboard";
                        for (int nField = 1; nField <= 64; nField++) //przewertuj szachownicę w jej poszukiwaniu
                        {
                            if (nStartPieceTypeOnCheckedField == _pChessboard->anBoard
                                    [_pChessboard->fieldNrToFieldPos(nField, ROW)] //jeżeli na tym wertowanym polu...
                                    [_pChessboard->fieldNrToFieldPos(nField, COLUMN)]  //...szachownicy jest bierka której szukamy
                                    && nCurentlyCheckedField != nField) //ale nie może to być oryginalna pozycja bierki (musi być...
                                //...na obcym polu startowym)
                            {
                                //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                                this->pieceMovingSequence(REGULAR,_pChessboard->fieldNrToFieldPos(nField, ROW),
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
                        //qDebug() << "Chess::resetPiecePositions(): put piece from checked field to its own empty start field";

                        //to przenieś ją na swoje pierwotne/startowe pole na szachownicy
                        this->pieceMovingSequence(REGULAR, _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
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
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW)] //...startowym bierki, która jest...
                                [_pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN)]; //...na polu aktualnie sprawdzanym.

                        if (nStartNrOf2ndPieceOn1stPieceOrigPos == nCurentlyCheckedField) //jeżeli pole docelowe bierki, która znajduje...
                            //...się na polu aktualnie sprawdzanym jest zajętę przez bierkę, której pole startowe znajduje sie na polu aktualnie...
                            //...sprawdzanym (2 takie bierki, których pola startowe są zajęte przez siebie nawzajem- bardzo rzadki przypadek)
                        {
                            qDebug() << "Chess::resetPiecePositions(): 2 pieces on each other start field. Remove 2nd, move 1st, restore 2nd.";

                            //to przenieś ją na chwilę na jej miejsce na obszarze bierek zbitych.
                            this->pieceMovingSequence(REMOVING, _pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN),
                                                      _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, COLUMN));

                            //teraz przenieś bierkę z pola aktualnie sprawdzanego, na pole bierki dopiero co odłożonej na obszar bierek zbitych
                            this->pieceMovingSequence(REGULAR, _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nCurentlyCheckedField, COLUMN),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, ROW),
                                                      _pChessboard->fieldNrToFieldPos(nPieceOrigPos, COLUMN));

                            //na końcu bierkę odłożoną poza planszę przywróć na pole bierki wcześniej przestawionej
                            this->pieceMovingSequence(RESTORE, _pChessboard->fieldNrToFieldPos(nNrOf2ndPieceOn1stPieceOrigPos, ROW),
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
        this->pieceMovingSequence(REGULAR);
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

MOVE_TYPE Chess::checkMoveType(QString move)
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

#include "chessboard.h"

Chessboard::Chessboard():
    anStartBoard{
{1, 9, 0, 0, 0, 0, 17, 25} ,
{2, 10, 0, 0, 0, 0, 18, 26} ,
{3, 11, 0, 0, 0, 0, 19, 27} ,
{4, 12, 0, 0, 0, 0, 20, 28} ,
{5, 13, 0, 0, 0, 0, 21, 29} ,
{6, 14, 0, 0, 0, 0, 22, 30} ,
{7, 15, 0, 0, 0, 0, 23, 31} ,
{8, 16, 0, 0, 0, 0, 24, 32}},

    anRemoved{
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0}},
    m_nMaxPieceHeight(52), // dla pola h8 max wysokość bierki to 46
    m_nMaxRemovedPieceH(44.5),
    m_QStrSiteMoveRequest(""),
    m_WhoseTurn(NO_TURN),
    lTimersStartTime(1000*60*30) //1000ms (1s) * 60s * 30min
{
    memcpy(anBoard, anStartBoard, sizeof(anStartBoard)); //pseudooperator anBoard = anStartBoard
    memcpy(anTempBoard, anStartBoard, sizeof(anStartBoard)); //pseudooperator anTempBoard = anStartBoard

    QsPiecieFromTo = "";
    nGripperPiece = 0;


    double a1_x = 168.8; double a1_y = 77.4; double a1_z = -21.9;
    double a8_x = 317.0; double a8_y = 71.8; double a8_z = -16.6;
    double h1_x = 164.7; double h1_y = -91.2; double h1_z = -21.5;
    double h8_x = 314.0; double h8_y = -85.5; double h8_z = -16.3;
    //                                      "z" to pozycje na styku chwytaka z szachownicą

    std::vector<double> v{ a1_z, a8_z, h1_z, h8_z };
    m_nMaxBoardZ = *std::max_element( v.begin(), v.end() );

    for (int digit = 0; digit <= 7; digit++)
    {
        for (int letter = 0; letter <= 7; letter++)
        {
            adChessboardPositions_x[letter][digit] = a1_x +
                    digit*(((a8_x-a1_x)/7.f)+((letter/14.f)*(((a1_x-h1_x)/7.f)-((a8_x-h8_x)/7.f))))-
                    letter*(((a1_x-h1_x)/7.f)-((digit/14.f)*(((h8_x-h1_x)/7.f)-((a8_x-a1_x)/7.f))));
            adChessboardPositions_y[letter][digit] = a1_y +
                    digit*(((a8_y-a1_y)/7.f)+((letter/14.f)*(((a1_y-h1_y)/7.f)-((a8_y-h8_y)/7.f))))-
                    letter*(((a1_y-h1_y)/7.f)-((digit/14.f)*(((h8_y-h1_y)/7.f)-((a8_y-a1_y)/7.f))));
            adChessboardPositions_z[letter][digit] = a1_z +
                    digit*(((a8_z-a1_z)/7.f)+((letter/14.f)*(((a1_z-h1_z)/7.f)-((a8_z-h8_z)/7.f))))-
                    letter*(((a1_z-h1_z)/7.f)-((digit/14.f)*(((h8_z-h1_z)/7.f)-((a8_z-a1_z)/7.f))));
        }
    }

    for (int column = 0; column <= 1; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            adRemovedPiecesPositions_x[row][column] = 115.f + row*((266.7-115.f)/7.f);
            adRemovedPiecesPositions_y[row][column] = 177.f - column*21.2 + (-7.8/7.f)*row; //169.2-177= -7.8
            adRemovedPiecesPositions_z[row][column] = -21.f - row*((-21.f + 20.f)/7.f);
        }
    }
    for (int column = 2; column <= 3; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            adRemovedPiecesPositions_x[row][column] = 118.7 + row*((271.5-118.7)/7.f);
            adRemovedPiecesPositions_y[row][column] = -160.7 + ((column-2)*(-20.3)) + (10.f/7.f)*row; //-181-(-160.7) = -20.3
            adRemovedPiecesPositions_z[row][column] = -20.4 - row*((-4.6)/7.f); //(-20.4 - (-15.8) = -4.6
        }
    }

    m_whiteTimer = new QTimer();
    m_blackTimer = new QTimer();
    m_updateLabelTimer = new QTimer();
    m_whiteTimer->setInterval(lTimersStartTime);
    m_blackTimer->setInterval(lTimersStartTime);
    m_updateLabelTimer->setInterval(1000);
    m_whiteTimer->setSingleShot(true);
    m_blackTimer->setSingleShot(true);
    m_updateLabelTimer->setSingleShot(false);
    connect(m_whiteTimer, SIGNAL(timeout()), this, SLOT(timeOutWhite()));
    connect(m_blackTimer, SIGNAL(timeout()), this, SLOT(timeOutBlack()));
    connect(m_updateLabelTimer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    m_nRemainingWhiteTime = lTimersStartTime;
    m_nRemainingBlackTime = lTimersStartTime;
}

void Chessboard::findBoardPos(QString QStrPiecePositions)
{
    m_QStrSiteMoveRequest = QStrPiecePositions; //niezbędne aktualnie powtórzenie

    PieceFrom.Letter = this->findPieceLetterPos(QStrPiecePositions.left(1)); //todo: czy uzywana jest dobra z dwóch dostepnych funkcji?
    PieceFrom.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(1,1).toInt() - 1);

    PieceTo.Letter = this->findPieceLetterPos(QStrPiecePositions.mid(2,1));
    PieceTo.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(3,1).toInt() - 1);
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Letter =" << PieceFrom.Letter <<
                ", PieceFrom.Digit =" << PieceFrom.Digit <<
                ", PieceTo.Letter =" << PieceTo.Letter <<
                ", PieceTo.Digit =" << PieceTo.Digit;
}

LETTER Chessboard::findPieceLetterPos(QString QsLetter)
{
    LETTER letter;

    if (QsLetter == "a" || QsLetter == "A") {letter = L_A; }
    else if (QsLetter == "b" || QsLetter == "B") {letter = L_B;}
    else if (QsLetter == "c" || QsLetter == "C") {letter = L_C;}
    else if (QsLetter == "d" || QsLetter == "D") {letter = L_D;}
    else if (QsLetter == "e" || QsLetter == "E") {letter = L_E;}
    else if (QsLetter == "f" || QsLetter == "F") {letter = L_F;}
    else if (QsLetter == "g" || QsLetter == "G") {letter = L_G;}
    else if (QsLetter == "h" || QsLetter == "H") {letter = L_H;}
    else qDebug() << "ERROR: Chessboard::findPieceLetterPos(QString QsLetter): "
                     "Unknown QsLetter value.";

    return letter;
}

QString Chessboard::findPieceLetterPos(LETTER letter)
{
    QString QstrLetter;

    switch(letter)
    {
    case L_A: QstrLetter = "a"; break;
    case L_B: QstrLetter = "b"; break;
    case L_C: QstrLetter = "c"; break;
    case L_D: QstrLetter = "d"; break;
    case L_E: QstrLetter = "e"; break;
    case L_F: QstrLetter = "f"; break;
    case L_G: QstrLetter = "g"; break;
    case L_H: QstrLetter = "h"; break;
    default:
        qDebug() << "ERROR: Chessboard::findPieceLetterPos(LETTER letter): Unknown letter value.";
        break;
    }

    return QstrLetter;
}

QString Chessboard::getPiecieFromTo()
{
    QString piecieFromTo = findPieceLetterPos(PieceFrom.Letter) + QString::number(PieceFrom.Digit+1) +
            findPieceLetterPos(PieceTo.Letter) + QString::number(PieceTo.Digit+1);

    return piecieFromTo;
}


int Chessboard::fieldNrToFieldPos(int nfieldNr, bool bRow) //będzie działać też dla bierek...
{ //...zbitych jako PieceToRemoveToRemovedPiecePos (0-32)
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
        emit this->addTextToConsole("ERROR. Chess::fieldNrToFieldPos: próba"
                                    " dzielenia przez zero.  nfieldNr = " +
                                    (QString)nfieldNr + ", bRow = " + bRow + "\n");
        qDebug() << "ERROR. Chess::fieldNrToFieldPos: proba dzielenia przez zero. nfieldNr =" <<
                    nfieldNr << ", bRow =" << bRow;
        return 0; //coś trzeba zwrócić
    }
}

bool Chessboard::isMoveRemoving()
{
    if (m_QStrBoard[PieceTo.Letter][PieceTo.Digit] != "0")
    {
        qDebug() << "m_QStrBoard[PieceTo.Letter:" << PieceTo.Letter << "][PieceTo.Digit:" <<
                    PieceTo.Digit << "] =" << m_QStrBoard[PieceTo.Letter][PieceTo.Digit];
        return true;
    }
    else return false;
}

bool Chessboard::isMoveCastling(QString moveToTest)
{    
    if ((moveToTest == "e1c1" && m_QStrCastlings.contains("Q") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e1g1" && m_QStrCastlings.contains("K") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e8c8" && m_QStrCastlings.contains("q") && m_QStrBoard[4][7] == "k") ||
            (moveToTest == "e8g8" && m_QStrCastlings.contains("k") && m_QStrBoard[4][7] == "k"))
        return true;
    else return false;
}

bool Chessboard::isMoveEnpassant(QString moveToTest)
{
    if (((m_QStrBoard[PieceFrom.Letter][PieceFrom.Digit] == "P" && m_WhoseTurn == WHITE_TURN) ||
         (m_QStrBoard[PieceFrom.Letter][PieceFrom.Digit] == "p" && m_WhoseTurn == BLACK_TURN)) &&
            moveToTest.right(2) == m_QStrEnpassant)
        return true;
    else return false;
}

void Chessboard::castlingFindRookToMove()
{
    //zmieniają się tylko litery, a cyrfy pozostają.
    if (PieceTo.Letter == L_C)
    {
        PieceFrom.Letter = L_A;
        PieceTo.Letter = L_D;
    }
    else if (PieceTo.Letter == L_G)
    {
        PieceFrom.Letter = L_H;
        PieceTo.Letter = L_F;
    }
    else qDebug() << "ERROR: wrong PieceTo.Letter in Chessboard::castlingFindRookToMove() =" << PieceTo.Letter;
}

void Chessboard::pieceStateChanged(DOBOT_MOVE partOfSequence, LETTER letter,
                                   DIGIT digit, SEQUENCE_TYPE Type)
{
    if (Type == RESTORE && partOfSequence == FROM) //jeżeli bierka została pochwycona z obszaru bierek zbitych...
    {
        nGripperPiece = anRemoved[letter][digit];
        //...to w chwytaku jest bierka z obszaru zbitych
        anRemoved[letter][digit] = 0; //miejsce ruszanego pionka jest już puste
        qDebug() << "Chessboard::pieceStateChanged: restore: removed field value shall now be 0. "
                    "aanRemoved[letter][digit]; = "
                 << anRemoved[letter][digit];
    }
    else if (Type == REMOVING && partOfSequence == TO) //jeżeli bierka została przemieszczona na...
        //...obszar bierek zbitych z szachownicy...
    {
        //nPieceLetter i nPieceDigit nie moga być podawane jako parametry pozycji bierki na...
        //...obszarze zbitych, bo są to pozycje na szachownicy. docelowe pozycje na obszarze...
        //...zbitych trzeba wyznaczyć na podstawie bierki trzymanej w chwytaku
        LETTER remPieceDestLetter = static_cast<LETTER>(this->fieldNrToFieldPos(nGripperPiece, ROW));
        DIGIT remPieceDestDigit = static_cast<DIGIT>(this->fieldNrToFieldPos(nGripperPiece, COLUMN));

        //...to pole tej bierki na obszarze bierek zbitych jest już przez nią zajęte...
        anRemoved[remPieceDestLetter][remPieceDestDigit] =  nGripperPiece;
        qDebug() << "Piece (>0) just placed on removed area ="
                 << anRemoved[remPieceDestLetter][remPieceDestDigit];
        nGripperPiece = 0; //...a chwytak nie trzyma już żadnej bierki
    }
    else if (partOfSequence == FROM) //...a jeżeli bierka została pochwycona z szachownicy...
        //...(jest to każde inne polecenie ruchu w stylu 'pieceFrom')...
    {
        nGripperPiece = anBoard[letter][digit]; //...to w chwytaku jest bierka...
        //...pochwycona z szachownicy...
        anBoard[letter][digit] = 0; //...a miejsce ruszanego pionka jest już puste.
    }

    else if (partOfSequence == TO)//lecz jeżeli bierka została przemieszczona na szachownicę
        //...(jest to każde inne polecenie ruchu w stylu 'pieceTo')...
    {
        anBoard[letter][digit] = nGripperPiece; //...to docelowe pole na...
        //...szachownicy jest już zajęte...
        nGripperPiece = 0; //... a w chwytaku nie ma już żadnej bierki.
    }
    else qDebug() << "ERROR: Chessboard::pieceStateChanged: none statement has been met.";
}

bool Chessboard::compareArrays(int nArray1[][8], int nArray2[][8])
{
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            if (nArray1[i][j] != nArray2[i][j])
                return false;
        }
    }
    return true;
}

void Chessboard::changeWindowTitle()
{
    QString title;

    switch(m_WhoseTurn)
    {
    case NO_TURN:
        title = "Awaiting for new game";
        break;
    case WHITE_TURN:
        title = "White's turn";
        break;
    case BLACK_TURN:
        title = "Black's turn";
        break;
    default:
        qDebug() << "ERROR: Chessboard::changeWindowTitle()- unknown m_WhoseTurn parameter";
        title = "ERROR";
    }

    emit this->changeWindowTitle(title);
}

void Chessboard::saveStatusData(QString status)
{
    QStringList QStrFENRecord = status.split(QRegExp("\\s+"));
    if (!QStrFENRecord.isEmpty()) QStrFENRecord.removeLast();

    if (QStrFENRecord.size() == 7)
    {
        m_QStrGameStatus = QStrFENRecord.at(0);
        qDebug() << "QStrGameStatus =" << m_QStrGameStatus;
        emit setBoardDataLabels(m_QStrGameStatus, BDL_GAME_STATUS);

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "QStrFENBoard =" << QStrFENBoard;
        FENToBoard(QStrFENBoard);
        emit showBoard(arrayBoardToQStr(m_QStrBoard));

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "QStrWhoseTurn =" << QStrWhoseTurn;
        this->setWhoseTurn(whoseTurn(QStrWhoseTurn));
        emit setBoardDataLabels(this->getStrWhoseTurn(), BDL_TURN);

        m_QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "QStrCastlings =" << m_QStrCastlings;
        emit setBoardDataLabels(m_QStrCastlings, BDL_CASTLINGS);

        m_QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "QStrEnpassant =" << m_QStrEnpassant;
        emit setBoardDataLabels(m_QStrEnpassant, BDL_ENPASSANT);

        QString QStrHalfmoveClock = QStrFENRecord.at(5);
        QString QStrFullmoveNumber = QStrFENRecord.at(6);
        emit setBoardDataLabels(QStrHalfmoveClock + "/" + QStrFullmoveNumber , BDL_MOVES);
    }
    else
    {
        qDebug() << "ERROR: Chessboard::saveStatusData: wrong QStrFENRecord size =" <<
                    QStrFENRecord.size();
        this->addTextToConsole("ERROR: Chessboard::saveStatusData: wrong QStrFENRecord size = "
                               + QStrFENRecord.size());
    }
}

void Chessboard::FENToBoard(QString FENBoard)
{
    QStringList QStrFENBoardRows = FENBoard.split(QRegExp("/"));
    std::reverse(QStrFENBoardRows.begin(), QStrFENBoardRows.end());
    if (QStrFENBoardRows.size() == 8)
    {
        QRegExp rxEmpty("\\d");
        for (int nRow=0; nRow<=7; ++nRow)
        {
            int nColumn = 0;
            QString QStrFENBoardRow = QStrFENBoardRows.at(nRow);
            QStringList FENSigns = QStrFENBoardRow.split("");
            FENSigns.removeFirst();
            FENSigns.removeLast();

            for (int nFENSignPos=0; nFENSignPos<FENSigns.size(); ++nFENSignPos)
            {
                QString QStrFENSign = FENSigns.at(nFENSignPos);
                if (!rxEmpty.exactMatch(QStrFENSign)) //not digits
                {
                    m_QStrBoard[nColumn][nRow] = QStrFENSign;
                    if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn > 8 =" << nColumn;
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        m_QStrBoard[nColumn][nRow] = "0";
                        if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn > 8 =" << nColumn;
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: Chessboard::FENToBoard: boardRows.size() != 8. Size = " << QStrFENBoardRows.size();
        for (int i=0; i<=QStrFENBoardRows.size()-1; ++i)
            qDebug() << "QStrFENBoardRows at" << i << "=" << QStrFENBoardRows.at(i);
    }

    this->showBoardInDebug();
}

WHOSE_TURN Chessboard::whoseTurn(QString whoseTurn)
{
    if (whoseTurn == "w") return WHITE_TURN;
    else if (whoseTurn == "b") return BLACK_TURN;
    else
    {
        return NO_TURN;
        qDebug () << "ERROR: Chessboard::whoseTurn- unknown turn type from status";
    }
}

void Chessboard::showBoardInDebug()
{
    for (int i=0; i<=7; ++i)
    {
        qDebug() << "Board's row" << i+1 << "pieces =" << m_QStrBoard[i][0] << m_QStrBoard[i][1] <<
                    m_QStrBoard[i][2] << m_QStrBoard[i][3] << m_QStrBoard[i][4] << m_QStrBoard[i][5] <<
                    m_QStrBoard[i][6] << m_QStrBoard[i][7];
    }
}

QString Chessboard::getStrWhoseTurn()
{
    if (m_WhoseTurn == WHITE_TURN) return "wt";
    else if (m_WhoseTurn == BLACK_TURN) return "bt";
    else if (m_WhoseTurn == NO_TURN) return "nt";
    else
    {
        QString err = "ERROR: wrong turn type: " + QString::number(m_WhoseTurn);
        return err;
    }
}

QString Chessboard::arrayBoardToQStr(QString QStrBoard[8][8])
{
    QString board = "";
    for (int i=0; i<=7; ++i)
    {
        for (int j=0; j<=7; ++j)
        {
            board += QStrBoard[i][j] + " ";
        }
        board += "\n";
    }
    return board;
}

void Chessboard::startGameTimer()
{
    m_whiteTimer->start();
    m_updateLabelTimer->start();
    m_updateLabelTimer->start();
}

void Chessboard::stopBoardTimers()
{
    m_whiteTimer->stop();
    m_blackTimer->stop();
    m_updateLabelTimer->stop();
}

void Chessboard::timeOutWhite()
{
    this->stopBoardTimers();
    emit msgFromChessboardToWebsockets("timeOutWhite");
}

void Chessboard::timeOutBlack()
{
    this->stopBoardTimers();
    emit msgFromChessboardToWebsockets("timeOutBlack");
}

void Chessboard::resetTimers()
{
    this->stopBoardTimers();
    m_whiteTimer->setInterval(lTimersStartTime);
    m_blackTimer->setInterval(lTimersStartTime);
    m_nRemainingWhiteTime = m_whiteTimer->remainingTime();
    m_nRemainingBlackTime = m_blackTimer->remainingTime();
    emit setBoardDataLabels(milisecToClockTime(lTimersStartTime), BDL_WHITE_TIME);
    emit setBoardDataLabels(milisecToClockTime(lTimersStartTime), BDL_BLACK_TIME);
}

QString Chessboard::milisecToClockTime(long lMilis)
{
    if (lMilis > 0)
    {
        int nAllSecs = lMilis / 1000;
        int nMins = qFloor(nAllSecs / 60);
        int nSecs = qFloor(nAllSecs % 60);
        QString QStrMinsPrefix, QStrSecsPrefix;
        nMins >= 10 ? QStrMinsPrefix == "" : QStrMinsPrefix == "0";
        nSecs >= 10 ? QStrSecsPrefix == "" : QStrSecsPrefix == "0";
        return QStrMinsPrefix + QString::number(nMins) + ":" + QStrSecsPrefix + QString::number(nSecs);
    }
    else return "00:00";
}

void Chessboard::updateTimeLabels()
{
    emit setBoardDataLabels(milisecToClockTime(m_nRemainingWhiteTime), BDL_WHITE_TIME);
    emit setBoardDataLabels(milisecToClockTime(m_nRemainingBlackTime), BDL_BLACK_TIME);
}

void Chessboard::switchPlayersTimers()
{
    if (getWhoseTurn() == WHITE_TURN)
    {
        m_nRemainingBlackTime = m_blackTimer->remainingTime();
        m_blackTimer->stop();

        m_whiteTimer->setInterval(m_nRemainingWhiteTime);
        m_whiteTimer->start();
    }
    else if (getWhoseTurn() == BLACK_TURN)
    {
        m_nRemainingWhiteTime = m_whiteTimer->remainingTime();
        m_whiteTimer->stop();

        m_blackTimer->setInterval(m_nRemainingBlackTime);
        m_blackTimer->start();
    }
    else qDebug() << "ERROR: Chessboard::switchPlayersTimers(): getWhoseTurn isn't "
                     "white or black.  it's' ==" << getWhoseTurn();
}

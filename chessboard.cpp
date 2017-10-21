#include "chessboard.h"

Chessboard::Chessboard():
    m_anBoardStart{
{1, 9, 0, 0, 0, 0, 17, 25} ,
{2, 10, 0, 0, 0, 0, 18, 26} ,
{3, 11, 0, 0, 0, 0, 19, 27} ,
{4, 12, 0, 0, 0, 0, 20, 28} ,
{5, 13, 0, 0, 0, 0, 21, 29} ,
{6, 14, 0, 0, 0, 0, 22, 30} ,
{7, 15, 0, 0, 0, 0, 23, 31} ,
{8, 16, 0, 0, 0, 0, 24, 32}},

    m_asBoardRemoved{
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
    m_lTimersStartTime(1000*60*30), //1000ms (1s) * 60s * 30min
    m_lTimersStartQueue(1000*60*2)
{
    memcpy(m_asBoardMain, m_anBoardStart, sizeof(m_anBoardStart)); //todo: pseudooperator m_asBoardMain == m_anBoardStart
    memcpy(m_asBoardTemp, m_anBoardStart, sizeof(m_anBoardStart)); //todo: pseudooperator m_asBoardTemp == m_anBoardStart

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
            m_adChessboardPositions_x[letter][digit] = a1_x +
                    digit*(((a8_x-a1_x)/7.f)+((letter/14.f)*(((a1_x-h1_x)/7.f)-((a8_x-h8_x)/7.f))))-
                    letter*(((a1_x-h1_x)/7.f)-((digit/14.f)*(((h8_x-h1_x)/7.f)-((a8_x-a1_x)/7.f))));
            m_adChessboardPositions_y[letter][digit] = a1_y +
                    digit*(((a8_y-a1_y)/7.f)+((letter/14.f)*(((a1_y-h1_y)/7.f)-((a8_y-h8_y)/7.f))))-
                    letter*(((a1_y-h1_y)/7.f)-((digit/14.f)*(((h8_y-h1_y)/7.f)-((a8_y-a1_y)/7.f))));
            m_adChessboardPositions_z[letter][digit] = a1_z +
                    digit*(((a8_z-a1_z)/7.f)+((letter/14.f)*(((a1_z-h1_z)/7.f)-((a8_z-h8_z)/7.f))))-
                    letter*(((a1_z-h1_z)/7.f)-((digit/14.f)*(((h8_z-h1_z)/7.f)-((a8_z-a1_z)/7.f))));
        }
    }

    for (int column = 0; column <= 1; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            m_adRemovedPiecesPositions_x[row][column] = 115.f + row*((266.7-115.f)/7.f);
            m_adRemovedPiecesPositions_y[row][column] = 177.f - column*21.2 + (-7.8/7.f)*row; //169.2-177= -7.8
            m_adRemovedPiecesPositions_z[row][column] = -21.f - row*((-21.f + 20.f)/7.f);
        }
    }
    for (int column = 2; column <= 3; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            m_adRemovedPiecesPositions_x[row][column] = 118.7 + row*((271.5-118.7)/7.f);
            m_adRemovedPiecesPositions_y[row][column] = -160.7 + ((column-2)*(-20.3)) + (10.f/7.f)*row; //-181-(-160.7) = -20.3
            m_adRemovedPiecesPositions_z[row][column] = -20.4 - row*((-4.6)/7.f); //(-20.4 - (-15.8) = -4.6
        }
    }

    m_whiteTimer = new QTimer();
    m_blackTimer = new QTimer();
    m_updateLabelTimer = new QTimer();
    m_startQueueTimer = new QTimer();
    m_whiteTimer->setInterval(m_lTimersStartTime);
    m_blackTimer->setInterval(m_lTimersStartTime);
    m_updateLabelTimer->setInterval(1000);
    m_startQueueTimer->setInterval(m_lTimersStartQueue);
    m_whiteTimer->setSingleShot(true);
    m_blackTimer->setSingleShot(true);
    m_updateLabelTimer->setSingleShot(false);
    m_startQueueTimer->setSingleShot(true);
    connect(m_whiteTimer, SIGNAL(timeout()), this, SLOT(timeOutWhite()));
    connect(m_blackTimer, SIGNAL(timeout()), this, SLOT(timeOutBlack()));
    connect(m_updateLabelTimer, SIGNAL(timeout()), this, SLOT(updateTimeLabels()));
    connect(m_startQueueTimer, SIGNAL(timeout()), this, SLOT(timeOutStartQueue()));
    m_nRemainingWhiteTime = m_lTimersStartTime;
    m_nRemainingBlackTime = m_lTimersStartTime;
}

void Chessboard::findBoardPos(QString QStrPiecePositions)
{
    m_QStrSiteMoveRequest = QStrPiecePositions; //niezbędne aktualnie powtórzenie

    PieceFrom.Letter = pieceLetterPos(QStrPiecePositions.left(1));
    PieceFrom.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(1,1).toInt() - 1);

    PieceTo.Letter = pieceLetterPos(QStrPiecePositions.mid(2,1));
    PieceTo.Digit = static_cast<DIGIT>(QStrPiecePositions.mid(3,1).toInt() - 1);
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Letter =" << PieceFrom.Letter <<
                ", PieceFrom.Digit =" << PieceFrom.Digit <<
                ", PieceTo.Letter =" << PieceTo.Letter <<
                ", PieceTo.Digit =" << PieceTo.Digit;
}

QString Chessboard::getPiecieFromTo()
{
    QString piecieFromTo = pieceLetterPos(PieceFrom.Letter) + QString::number(PieceFrom.Digit+1) +
            pieceLetterPos(PieceTo.Letter) + QString::number(PieceTo.Digit+1);

    return piecieFromTo;
}

short Chessboard::fieldLinesPosToFieldNr(FieldLinesPos fieldLines)
{
    short sFieldNr = static_cast<short>(fieldLines.Letter) +
            static_cast<short>(fieldLines.Digit)*8;
    if (sFieldNr < 1 || sFieldNr > 64)
    {
        qDebug() << "ERROR: Chessboard::fieldLinesPosToFieldNr: sFieldNr out of range <1,64>:"
                 << sFieldNr;
        return 0;
    }
    else return sFieldNr;
}

FieldLinesPos Chessboard::fieldNrToFieldLinesPos(short sFieldNr) //będzie działać też dla bierek...
{ //...zbitych jako PieceToRemoveToRemovedPiecePos (0-32)
    FieldLinesPos fieldLines;

    if (sFieldNr != 0) //zabezpieczenie przed przypadkowym podaniem zera do mianownika
    {
        short sFieldNrColumn, sFieldNrRow; //tablica[nfieldNrRow][nfieldNrColumn];

        if (sFieldNr % 8 != 0) //wszystkie prócz liczb 8, 16, 24, 32, 40, 48, 56 i 64.
        {
            sFieldNrColumn = sFieldNr / 8;
            sFieldNrRow  = (sFieldNr - 1) - (sFieldNrColumn * 8);
        }
        else //dla liczb 8, 16, 24, 32, 40, 48, 56 i 64.
        {
            sFieldNrColumn = (sFieldNr / 8) - 1;
            sFieldNrRow = 7;
        }

        fieldLines.Letter = static_cast<LETTER>(sFieldNrRow);
        fieldLines.Digit = static_cast<DIGIT>(sFieldNrColumn);
    }
    else
    {
        emit this->addTextToConsole("ERROR. Chess::fieldNrToFieldLinesPos: próba"
                                    " dzielenia przez zero.  nfieldNr = " +
                                    QString::number(sFieldNr) + "\n");
        qDebug() << "ERROR: Chessboard::fieldNrToFieldLinesPos: proba dzielenia przez zero. nfieldNr =" <<
                    sFieldNr;
    }

    return fieldLines;
}

bool Chessboard::isMoveRemoving()
{
    if (m_QStrBoard[PieceTo.Letter][PieceTo.Digit] != ".")
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
    if (Type == ST_RESTORE && partOfSequence == DM_FROM)
    {
        nGripperPiece = m_asBoardRemoved[letter][digit];
        qDebug() << "Chessboard::pieceStateChanged: 1) nGripperPiece =" << nGripperPiece;
        m_asBoardRemoved[letter][digit] = 0;
    }
    else if (Type == ST_REMOVING && partOfSequence == DM_TO)
    {
        this->setPieceOnBoard(B_REMOVED, nGripperPiece, nGripperPiece);
        qDebug() << "Piece just placed on removed area";
        nGripperPiece = 0;
        qDebug() << "Chessboard::pieceStateChanged: 2) nGripperPiece = 0";
    }
    //todo: bierki poruszane ramieniem zapisywane są do tablicy m_asBoardMain. jeżeli bierki zostały poruszone tylko w...
    //...pamięci bez udziału dobota (np. poprzez podane listy ruchów bezpośrednio do tcp) to system myśli że...
    //...plansza nie była ruszana, a co innego zobaczy się z polecenia "status".
    else if (partOfSequence == DM_FROM) //jeżeli bierka została pochwycona z szachownicy...
        //...(jest to każde inne polecenie ruchu w stylu 'pieceFrom')...
    {
        nGripperPiece = m_asBoardMain[letter][digit]; //...to w chwytaku jest bierka...
        //...pochwycona z szachownicy...
        qDebug() << "Chessboard::pieceStateChanged: 3) nGripperPiece =" << nGripperPiece;
        m_asBoardMain[letter][digit] = 0; //...a miejsce ruszanego pionka jest już puste.
    }
    else if (partOfSequence == DM_TO)//lecz jeżeli bierka została przemieszczona na szachownicę
        //...(jest to każde inne polecenie ruchu w stylu 'pieceTo')...
    {
        m_asBoardMain[letter][digit] = nGripperPiece; //...to docelowe pole na...
        //...szachownicy jest już zajęte...
        nGripperPiece = 0; //... a w chwytaku nie ma już żadnej bierki.
        qDebug() << "Chessboard::pieceStateChanged: 4) nGripperPiece = 0";
    }
    else qDebug() << "ERROR: Chessboard::pieceStateChanged: none statement has been met.";
}

bool Chessboard::compareArrays(short nArray1[][8], short nArray2[][8])
{
    for (short i=0; i<8; i++)
    {
        for (short j=0; j<8; j++)
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
                        m_QStrBoard[nColumn][nRow] = ".";
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
}

void Chessboard::stopBoardTimers()
{
    m_whiteTimer->stop();
    m_blackTimer->stop();
    m_updateLabelTimer->stop();
}

void Chessboard::timeOutWhite()
{
    this->resetGameTimers();
    emit msgFromChessboardToWebsockets("timeOutWhite");
}

void Chessboard::timeOutBlack()
{
    this->resetGameTimers();
    emit msgFromChessboardToWebsockets("timeOutBlack");
}

void Chessboard::resetGameTimers()
{
    this->stopBoardTimers();
    m_whiteTimer->setInterval(m_lTimersStartTime);
    m_blackTimer->setInterval(m_lTimersStartTime);
    m_nRemainingWhiteTime = m_whiteTimer->interval();
    m_nRemainingBlackTime = m_blackTimer->interval();
    emit setBoardDataLabels(milisecToClockTime(m_lTimersStartTime), BDL_WHITE_TIME);
    emit setBoardDataLabels(milisecToClockTime(m_lTimersStartTime), BDL_BLACK_TIME);
}

QString Chessboard::milisecToClockTime(long lMilis)
{
    if (lMilis > 0)
    {
        int nAllSecs = lMilis / 1000;
        int nMins = qFloor(nAllSecs / 60);
        int nSecs = qFloor(nAllSecs % 60);
        QString QStrMinsPrefix, QStrSecsPrefix;
        nMins >= 10 ? QStrMinsPrefix = "" : QStrMinsPrefix = "0";
        nSecs >= 10 ? QStrSecsPrefix = "" : QStrSecsPrefix = "0";
        return QStrMinsPrefix + QString::number(nMins) + ":" + QStrSecsPrefix + QString::number(nSecs);
    }
    else return "00:00";
}

void Chessboard::updateTimeLabels()
{
    if (m_whiteTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_whiteTimer->remainingTime()), BDL_WHITE_TIME);
    else if (m_blackTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_blackTimer->remainingTime()), BDL_BLACK_TIME);

    if (m_startQueueTimer->isActive())
        emit setBoardDataLabels(milisecToClockTime(m_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    else setBoardDataLabels(milisecToClockTime(m_lTimersStartQueue), BDL_QUEUE_TIME);
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

void Chessboard::startQueueTimer() //todo: zmienić nazewnictwo na "start" z wszystkich metod "queue"
{
    qDebug() << "Chessboard::startQueueTimer()";
    m_startQueueTimer->stop();
    m_startQueueTimer->setInterval(m_lTimersStartQueue);
    emit setBoardDataLabels(milisecToClockTime(m_startQueueTimer->remainingTime()), BDL_QUEUE_TIME);
    m_startQueueTimer->start();
    m_updateLabelTimer->start();
}

void Chessboard::stopQueueTimer()
{
    qDebug() << "Chessboard::stopQueueTimer()";
    m_startQueueTimer->stop();
    m_updateLabelTimer->stop();
    this->updateTimeLabels();
}

void Chessboard::timeOutStartQueue()
{
    this->stopQueueTimer();
    emit msgFromChessboardToWebsockets("timeOutStartQueue");
}

int Chessboard::getWhiteTimeLeft()
{
    if (m_whiteTimer->remainingTime() != -1)
        return m_whiteTimer->remainingTime();
    else return m_nRemainingWhiteTime;
}

int Chessboard::getBlackTimeLeft()
{
    if (m_blackTimer->remainingTime() != -1)
        return m_blackTimer->remainingTime();
    else return m_nRemainingBlackTime;
}

/*short Chessboard::getBoardByItsType(BOARD boardType)
{
    short asBoard[8][8];
    switch(boardType)
    {
    case B_MAIN: memcpy(asBoard, m_asBoardMain, sizeof(m_asBoardMain)); break;
    case B_START: memcpy(asBoard, m_anBoardStart, sizeof(m_anBoardStart)); break;
    case B_TEMP: memcpy(asBoard, m_asBoardTemp, sizeof(m_anBoardStart)); break;
    case B_REMOVED: memcpy(asBoard, m_asBoardRemoved, sizeof(m_anBoardStart)); break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }

    return asBoard; //todo: nie umiem zwrócić tej tablicy:
    //https://stackoverflow.com/questions/720707/how-to-return-two-dimensional-char-array-c
}*/

//todo: templaty/szablony?
void Chessboard::setPieceOnBoard(BOARD boardType, short sPieceNr, FieldLinesPos fieldLines)
{
    switch(boardType)
    {
    case B_MAIN: m_asBoardMain[fieldLines.Letter][fieldLines.Digit] = sPieceNr; break;
    case B_START: m_anBoardStart[fieldLines.Letter][fieldLines.Digit] = sPieceNr; break;
    case B_TEMP: m_asBoardTemp[fieldLines.Letter][fieldLines.Digit] = sPieceNr; break;
    case B_REMOVED: m_asBoardRemoved[fieldLines.Letter][fieldLines.Digit] = sPieceNr; break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }
}

void Chessboard::setPieceOnBoard(BOARD boardType, short sPieceNr, short sFieldNr)
{
    qDebug() << "fieldNrToFieldLinesPos10";
    FieldLinesPos fieldLines = fieldNrToFieldLinesPos(sFieldNr);
    this->setPieceOnBoard(boardType, sPieceNr, fieldLines);
}

void Chessboard::setPieceOnBoard(BOARD boardType, FieldLinesPos pieceLines, short sFieldNr)
{
    qDebug() << "fieldNrToFieldLinesPos11";
    FieldLinesPos fieldLines = fieldNrToFieldLinesPos(sFieldNr);
    short sPieceNr = fieldLinesPosToFieldNr(pieceLines);
    this->setPieceOnBoard(boardType, sPieceNr, fieldLines);
}

void Chessboard::setPieceOnBoard(BOARD boardType, FieldLinesPos pieceLines, FieldLinesPos fieldLines)
{
    short sPieceNr = fieldLinesPosToFieldNr(pieceLines);
    this->setPieceOnBoard(boardType, sPieceNr, fieldLines);
}

short Chessboard::getPieceOnBoardAsNr(BOARD boardType, short sFieldNr)
{
    qDebug() << "Chessboard::getPieceOnBoardAsNr: sFieldNr =" << sFieldNr;
    short asBoard[8][8]; //= getBoardByItsType(boardType);
    //todo: problem powtarzania kodu w/z getBoardByItsType
    switch(boardType)
    {
    case B_MAIN: memcpy(asBoard, m_asBoardMain, sizeof(m_asBoardMain)); break;
    case B_START: memcpy(asBoard, m_anBoardStart, sizeof(m_anBoardStart)); break;
    case B_TEMP: memcpy(asBoard, m_asBoardTemp, sizeof(m_anBoardStart)); break;
    case B_REMOVED: memcpy(asBoard, m_asBoardRemoved, sizeof(m_anBoardStart)); break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }

    qDebug() << "fieldNrToFieldLinesPos12";
    FieldLinesPos fieldLines = fieldNrToFieldLinesPos(sFieldNr);
    qDebug() << "fieldLines.Letter =" << (short)fieldLines.Letter << ", fieldLines.Digit =" <<
                (short)fieldLines.Digit << ", asBoard[][] =" <<
                asBoard[(short)fieldLines.Letter][(short)fieldLines.Digit] << ", f.e.: m_asBoardMain[][] =" <<
                m_asBoardMain[(short)fieldLines.Letter][(short)fieldLines.Digit];
    return asBoard[(short)fieldLines.Letter][(short)fieldLines.Digit];
}

short Chessboard::getPieceOnBoardAsNr(BOARD boardType, FieldLinesPos fieldLines)
{
    short asBoard[8][8]; //= getBoardByItsType(boardType);
    switch(boardType)
    {
    case B_MAIN: memcpy(asBoard, m_asBoardMain, sizeof(m_asBoardMain)); break;
    case B_START: memcpy(asBoard, m_anBoardStart, sizeof(m_anBoardStart)); break;
    case B_TEMP: memcpy(asBoard, m_asBoardTemp, sizeof(m_anBoardStart)); break;
    case B_REMOVED: memcpy(asBoard, m_asBoardRemoved, sizeof(m_anBoardStart)); break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }
    return asBoard[fieldLines.Letter][fieldLines.Digit];
}

FieldLinesPos Chessboard::getPieceOnBoardAsLines(BOARD boardType, short sFieldNr)
{
    qDebug() << "Chessboard::getPieceOnBoardAsNr: sFieldNr =" << sFieldNr;
    short asBoard[8][8]; //= getBoardByItsType(boardType);
    switch(boardType)
    {
    case B_MAIN: memcpy(asBoard, m_asBoardMain, sizeof(m_asBoardMain)); break;
    case B_START: memcpy(asBoard, m_anBoardStart, sizeof(m_anBoardStart)); break;
    case B_TEMP: memcpy(asBoard, m_asBoardTemp, sizeof(m_anBoardStart)); break;
    case B_REMOVED: memcpy(asBoard, m_asBoardRemoved, sizeof(m_anBoardStart)); break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }
    qDebug() << "fieldNrToFieldLinesPos13";
    FieldLinesPos fieldLines = fieldNrToFieldLinesPos(sFieldNr);
    short sPieceNr = asBoard[fieldLines.Letter][fieldLines.Digit];
    qDebug() << "fieldNrToFieldLinesPos14";
    FieldLinesPos pieceLines = fieldNrToFieldLinesPos(sPieceNr);
    return pieceLines;
}

FieldLinesPos Chessboard::getPieceOnBoardAsLines(BOARD boardType, FieldLinesPos fieldLines)
{
    short asBoard[8][8]; //= getBoardByItsType(boardType);
    switch(boardType)
    {
    case B_MAIN: memcpy(asBoard, m_asBoardMain, sizeof(m_asBoardMain)); break;
    case B_START: memcpy(asBoard, m_anBoardStart, sizeof(m_anBoardStart)); break;
    case B_TEMP: memcpy(asBoard, m_asBoardTemp, sizeof(m_anBoardStart)); break;
    case B_REMOVED: memcpy(asBoard, m_asBoardRemoved, sizeof(m_anBoardStart)); break;
    default: qDebug() << "ERROR: Chessboard::setPieceOnBoard: unknown BOARD val =" << boardType;
        break;
    }
    short sPieceNr = asBoard[fieldLines.Letter][fieldLines.Digit];
    qDebug() << "fieldNrToFieldLinesPos15";
    FieldLinesPos pieceLines = fieldNrToFieldLinesPos(sPieceNr);
    return pieceLines;
}

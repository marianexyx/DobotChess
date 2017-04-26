#include "chessboard.h"

#define ROW 1
#define COLUMN 0

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
{0, 0, 0, 0}}
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
}

void Chessboard::findBoardPos(QString QsPiecePositions)
{
    PieceFrom.Letter = this->findPieceLetterPos(QsPiecePositions.left(1));
    PieceFrom.Digit = QsPiecePositions.mid(1,1).toInt() - 1;

    PieceTo.Letter = this->findPieceLetterPos(QsPiecePositions.mid(2,1));
    PieceTo.Digit = QsPiecePositions.mid(3,1).toInt() - 1;
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Letter =" << PieceFrom.Letter <<
                ", PieceFrom.Digit =" << PieceFrom.Digit <<
                ", PieceTo.Letter =" << PieceTo.Letter <<
                ", PieceTo.Digit =" << PieceTo.Digit;
}

int Chessboard::findPieceLetterPos(QString QsLetter)
{
    int nLetter;

    if (QsLetter == "a" || QsLetter == "A") {nLetter = 0; }
    else if (QsLetter == "b" || QsLetter == "B") {nLetter = 1;}
    else if (QsLetter == "c" || QsLetter == "C") {nLetter = 2;}
    else if (QsLetter == "d" || QsLetter == "D") {nLetter = 3;}
    else if (QsLetter == "e" || QsLetter == "E") {nLetter = 4;}
    else if (QsLetter == "f" || QsLetter == "F") {nLetter = 5;}
    else if (QsLetter == "g" || QsLetter == "G") {nLetter = 6;}
    else if (QsLetter == "h" || QsLetter == "H") {nLetter = 7;}
    else qDebug() << "ERROR: Chessboard::findPieceLetterPos(QString QsLetter): "
                     "Unknown QsLetter value.";

    return nLetter;
}

QString Chessboard::findPieceLetterPos(int nLetter)
{
    QString QsLetter;

    switch(nLetter)
    {
    case 0: QsLetter = "a"; break;
    case 1: QsLetter = "b"; break;
    case 2: QsLetter = "c"; break;
    case 3: QsLetter = "d"; break;
    case 4: QsLetter = "e"; break;
    case 5: QsLetter = "f"; break;
    case 6: QsLetter = "g"; break;
    case 7: QsLetter = "h"; break;
    default:
        qDebug() << "ERROR: Chessboard::findPieceLetterPos(int nLetter): Unknown nLetter value.";
        break;
    }

    return QsLetter;
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

void Chessboard::castlingFindRookToMove() //ustawiane skąd-dokąd przenoszona będzie wieża w roszadzie.
{ //cyfra pola na którą wyląduje wieża pozostaje taka sama jak u króla.
    if (PieceTo.Letter == 2)
    {
        PieceFrom.Letter = 0;
        PieceTo.Letter = 3;
    }
    else
    {
        PieceFrom.Letter = 7;
        PieceTo.Letter = 5;
    }
}

void Chessboard::pieceStateChanged(bool bIsMoveFrom, int nPieceLetter,
                                   int nPieceDigit, MOVE_TYPE Type)
{
    if (Type == RESTORE && bIsMoveFrom) //jeżeli bierka została pochwycona z obszaru bierek zbitych...
    {
        nGripperPiece = anRemoved[nPieceLetter][nPieceDigit];
        //...to w chwytaku jest bierka z obszaru zbitych
        anRemoved[nPieceLetter][nPieceDigit] = 0; //miejsce ruszanego pionka jest już puste
        qDebug() << "Chessboard::pieceStateChanged: restore: removed field value shall now be 0. "
                    "anRemoved[nPieceLetter][nPieceDigit] = "
                 << anRemoved[nPieceLetter][nPieceDigit];
    }
    else if (Type == REMOVING && !bIsMoveFrom) //jeżeli bierka została przemieszczona na...
        //...obszar bierek zbitych z szachownicy...
    {
        //nPieceLetter i nPieceDigit nie moga być podawane jako parametry pozycji bierki na...
        //...obszarze zbitych, bo są to pozycje na szachownicy. docelowe pozycje na obszarze...
        //...zbitych trzeba wyznaczyć na podstawie bierki trzymanej w chwytaku
        int nRemPieceDestLetter = this->fieldNrToFieldPos(nGripperPiece, ROW);
        int nRemPieceDestDigit = this->fieldNrToFieldPos(nGripperPiece, COLUMN);

        //...to pole tej bierki na obszarze bierek zbitych jest już przez nią zajęte...
        anRemoved[nRemPieceDestLetter][nRemPieceDestDigit] =  nGripperPiece;
        qDebug() << "Piece (>0) just placed on removed area ="
                 << anRemoved[nRemPieceDestLetter][nRemPieceDestDigit];
        nGripperPiece = 0; //...a chwytak nie trzyma już żadnej bierki
    }
    else if (bIsMoveFrom) //...a jeżeli bierka została pochwycona z szachownicy...
        //...(jest to każde inne polecenie ruchu w stylu 'pieceFrom')...
    {
        nGripperPiece = anBoard[nPieceLetter][nPieceDigit]; //...to w chwytaku jest bierka...
        //...pochwycona z szachownicy...
        anBoard[nPieceLetter][nPieceDigit] = 0; //...a miejsce ruszanego pionka jest już puste.
    }

    else if (!bIsMoveFrom)//lecz jeżeli bierka została przemieszczona na szachownicę
        //...(jest to każde inne polecenie ruchu w stylu 'pieceTo')...
    {
        anBoard[nPieceLetter][nPieceDigit] = nGripperPiece; //...to docelowe pole na...
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
        qDebug() << "QStrFENRecord.size() == 7";
        m_QStrGameStatus = QStrFENRecord.at(0);
        qDebug() << "QStrGameStatus =" << m_QStrGameStatus;

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "QStrFENBoard =" << QStrFENBoard;
        FENToBoard(QStrFENBoard);

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "QStrWhoseTurn =" << QStrWhoseTurn;
        this->setWhoseTurn(whoseTurn(QStrWhoseTurn));

        m_QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "QStrCastlings =" << m_QStrCastlings;

        m_QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "QStrEnpassant =" << m_QStrEnpassant;

        //QString QStrHalfmoveClock = QStrFENRecord.at(5); //przyda sie na przyszlosc
        //QString QStrFullmoveNumber = QStrFENRecord.at(6);
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
                    if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn>8 =" << nColumn;
                    ++nColumn;
                }
                else //digits
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        m_QStrBoard[nColumn][nRow] = "0";
                        if (nColumn>7) qDebug() << "ERROR: Chessboard::FENToBoard: nColumn>8 =" << nColumn;
                        ++nColumn;
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "ERROR: IgorBot::checkMsgFromChenard: boardRows.size() != 8. Size = " << QStrFENBoardRows.size();
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
        qDebug () << "ERROR: IgorBot::checkMsgFromChenard- unknown turn type from status";
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

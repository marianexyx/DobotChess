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

    m_WhoseTurn = NO_TURN;

    QsPiecieFromTo = "";
    nGripperPiece = 0;

    bTestEnpassant = false;

    bTestPromotion = false;
    QsFuturePromote = "";
    bPromotionConfirmed = false;

    double a1_x = 169.7; double a1_y = 76.0; double a1_z = -20.8;
    double a8_x = 317.6; double a8_y = 70.6; double a8_z = -15.6;
    double h1_x = 163.8; double h1_y = -92.2; double h1_z = -20.6;
    double h8_x = 317.6; double h8_y = -87.3; double h8_z = -16.1;
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

    /*prawa strona:
    x = 100 (skok po 25 mm)
    rząd bliżej środka: y = -155
    rząd zewnętrzny:    y = -185
    z_max = 56
    dla x = 100, y = -155  wynik z = -22.5
    dla x = 275, y = -185  wynik z = -17

    środek: x = 130, y = 0

    lewa strona zatem:
    x = 100 (skok po 25 mm)
    rząd bliżej środka: y = 140
    rząd zewnętrzny:    y = 170
    z_max = 40 (a dla y = 145 -> z_max = 44.5)
    dla x = 100, y = 140  wynik z = -22.3
    dla x = 275, y = 170  wynik z = -16.5*/

    for (int column = 0; column <= 1; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            adRemovedPiecesPositions_x[row][column] = 125 + row*25;
            adRemovedPiecesPositions_y[row][column] = 170 - column*25;
            adRemovedPiecesPositions_z[row][column] = -22.3 - row*((-22.3 + 16.5)/7);
        }
    }
    for (int column = 2; column <= 3; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            adRemovedPiecesPositions_x[row][column] = 125 + row*25;
            adRemovedPiecesPositions_y[row][column] = -160 - ((column-2)*25);
            adRemovedPiecesPositions_z[row][column] = -22.5 - row*((-22.5 + 17)/7);
        }
    }
}

void Chessboard::findBoardPos(QString QsPiecePositions)
{
    PieceFrom.Letter = this->findPieceLetterPos(QsPiecePositions.left(1));
    PieceFrom.Digit = QsPiecePositions.mid(1,1).toInt() - 1;
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Digit =" << PieceFrom.Digit;

    PieceTo.Letter = this->findPieceLetterPos(QsPiecePositions.mid(2,1));
    PieceTo.Digit = QsPiecePositions.mid(3,1).toInt() - 1;
    qDebug() << "Chessboard::findBoardPos: PieceTo.Digit =" << PieceTo.Digit;
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
        emit this->addTextToConsole("ERROR. Chess::fieldNrToFieldPos: "
                                    "próba dzielenia przez zero \n");
        qDebug() << "ERROR. Chess::fieldNrToFieldPos: proba dzielenia przez zero";
        return 0; //coś trzeba zwrócić
    }
}


bool Chessboard::isMoveRemoving()
{
    if (m_QStrBoard[PieceTo.Letter][PieceTo.Digit] != "0") return true;
    else return false;
}

bool Chessboard::isMoveCastling(QString moveToTest)
{    
    if ((moveToTest == "e1c1" && m_QStrCastlings.contains("Q") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e1g1" && m_QStrCastlings.contains("K") && m_QStrBoard[4][0] == "K") ||
            (moveToTest == "e8c8" && m_QStrCastlings.contains("q") && m_QStrBoard[4][0] == "k") ||
            (moveToTest == "e8g8" && m_QStrCastlings.contains("k") && m_QStrBoard[4][0] == "k"))
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
                                   int nPieceDigit, char chMoveType)
{
    if (chMoveType == 's' && bIsMoveFrom) //jeżeli bierka została pochwycona z obszaru bierek zbitych...
    {
        nGripperPiece = anRemoved[nPieceLetter][nPieceDigit];
        //...to w chwytaku jest bierka z obszaru zbitych
        anRemoved[nPieceLetter][nPieceDigit] = 0; //miejsce ruszanego pionka jest już puste
        qDebug() << "Chessboard::pieceStateChanged: restore: removed field value shall now be 0. "
                    "anRemoved[nPieceLetter][nPieceDigit] = "
                 << anRemoved[nPieceLetter][nPieceDigit];
    }
    else if (chMoveType == 'r' && !bIsMoveFrom) //jeżeli bierka została przemieszczona na...
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
    //rozbicie statusu
    QStringList QStrFENRecord = status.split(QRegExp("\\s")); //spacja

    if (QStrFENRecord.size() == 7)
    {
        m_QStrGameStatus = QStrFENRecord.at(0);
        qDebug() << "QStrGameStatus =" << m_QStrGameStatus;

        QString QStrFENBoard = QStrFENRecord.at(1);
        qDebug() << "QStrFENBoard =" << QStrFENBoard;
        m_QStrBoard = FENToBoard(QStrFENBoard);

        QString QStrWhoseTurn = QStrFENRecord.at(2);
        qDebug() << "QStrWhoseTurn =" << QStrWhoseTurn;
        this->setWhoseTurn(whoseTurn(QStrWhoseTurn)); //todo: sprawdzić to

        m_QStrCastlings = QStrFENRecord.at(3);
        qDebug() << "QStrCastlings =" << m_QStrCastlings;

        m_QStrEnpassant = QStrFENRecord.at(4);
        qDebug() << "QStrenpassant =" << m_QStrEnpassant;

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

/*void Chessboard::gameState(QString QStrGameState)
{
    if (QStrGameStatus == "*") this->GameInProgress();
    else  if (QStrGameStatus == "1-0" || QStrGameStatus == "0-1" || QStrGameStatus == "1/2-1/2")
        this->EndOfGame(QStrGameStatus);
    else wrongTcpAnswer(tcpMsgType, tcpRespond);
}*/

QString Chessboard::FENToBoard(QString FENBoard)
{
    //rozmieszczenie na planszy
    QString aBoard[8][8];

    QStringList QStrFENBoardRows = QStrFENBoard.split("/");
    if (QStrFENBoardRows.size() == 8)
    {
        QRegExp rxEmpty("\d");
        for (int nRow=0; nRow<=7; ++nRow)
        {
            int nColumn = 0;
            QStringList FENSigns = QStrFENBoardRows.split(".");
            for (int nFENSignPos=1; nFENSignPos<=FENSigns.size(); ++nFENSignPos)
            {
                QString QStrFENSign = FENSigns.at(nFENSignPos);
                if (!rxEmpty.exactMatch(QStrFENSign))
                {
                    ++nColumn;
                    aBoard[nRow][nColumn] = QStrFENSign;
                }
                else
                {
                    for (int nEmptyFields=1; nEmptyFields<=QStrFENSign.toInt(); ++nEmptyFields)
                    {
                        ++nColumn;
                        aBoard[nRow][nColumn] = "0";
                    }
                }
                if (nColumn>7)
                    qDebug() << "ERROR: IgorBot::checkMsgFromChenard: nColumn>8 =" << nColumn;
            }
        }
    }
    else qDebug() << "ERROR: IgorBot::checkMsgFromChenard: boardRows.size() != 8";

    for (int i=0; i<=7; ++i)
    {
        qDebug() << "Board's row" << i+1 << "pieces =" << aBoard[i][0] << aBoard[i][1] <<
                    aBoard[i][2] << aBoard[i][3] << aBoard[i][4] << aBoard[i][5] <<
                    aBoard[i][6] << aBoard[i][7];
    }

    return aBoard;
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

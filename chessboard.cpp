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

    double a1_x = 172.4; double a1_y = 76.5; double a1_z = -20.5;
    double a8_x = 319.1; double a8_y = 70.3; double a8_z = -16.3;
    double h1_x = 165.5; double h1_y = -93.6; double h1_z = -21.1;
    double h8_x = 315.1; double h8_y = -87.3; double h8_z = -16.1;
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
            adRemovedPiecesPositions_x[row][column] = 100 + row*25;
            adRemovedPiecesPositions_y[row][column] = 170 - column*25;
            adRemovedPiecesPositions_z[row][column] = -22.3 - row*((-22.3 + 16.5)/7);
        }
    }
    for (int column = 2; column <= 3; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            adRemovedPiecesPositions_x[row][column] = 100 + row*25;
            adRemovedPiecesPositions_y[row][column] = -160 - ((column-2)*25);
            adRemovedPiecesPositions_z[row][column] = -22.5 - row*((-22.5 + 17)/7);
        }
    }
}

void Chessboard::findBoardPos(QString QsPiecePositions)
{
    QsPiecieFromTo = QsPiecePositions.mid(5,4); // e2e4. wycięte z "move e2e4".
    qDebug() << "QsPiecieFromTo: " << QsPiecieFromTo;

    PieceFrom.Letter = this->findPieceLetterPos(QsPiecePositions.mid(5,1));
    PieceFrom.Digit = QsPiecePositions.mid(6,1).toInt() - 1;
    qDebug() << "Chessboard::findBoardPos: PieceFrom.Digit =" << PieceFrom.Digit;

    PieceTo.Letter = this->findPieceLetterPos(QsPiecePositions.mid(7,1));
    PieceTo.Digit = QsPiecePositions.mid(8,1).toInt() - 1;
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


bool Chessboard::removeStatements() // funkcje do sprawdzania czy bijemy bierkę
{
    //sprawdzanie czy na pole, gdzie bierka idzie nie jest zajęte
    //w enpassant bierka "PieceTo" jest podmiania na pozycję bierki zbijanej, także ten warunek...
    //...tam też zadziała
    if (anBoard[PieceTo.Letter][PieceTo.Digit] != 0) return 1;
    else return 0;
}

bool Chessboard::castlingStatements() // sprawdzanie czy dany ruch jest prośbą o roszadę
{    
    if (PieceFrom.Letter == 4 && (PieceFrom.Digit == 0 || PieceFrom.Digit == 7)
            //jeżeli ruszana jest bierka z pozycji króla
            && (PieceTo.Letter == 2 || PieceTo.Letter == 6) // o 2 pola w lewo lub prawo
            && (PieceTo.Digit == 0 || PieceTo.Digit == 7) //na tej samej linii
            && (anBoard[PieceFrom.Letter][PieceFrom.Digit] == 5 //i jest to na pewno król (biały)
                || anBoard[PieceFrom.Letter][PieceFrom.Digit] == 29)) //(lub czarny)
    {
        return 1; //to mamy do czynienia z roszadą
    }
    else
        return 0; //a jak nie to nie
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

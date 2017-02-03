#include "chessboard.h"

Chessboard::Chessboard():
    anStartBoard{{1, 9, 0, 0, 0, 0, 17, 25} ,
{2, 10, 0, 0, 0, 0, 18, 26} ,
{3, 11, 0, 0, 0, 0, 19, 27} ,
{4, 12, 0, 0, 0, 0, 20, 28} ,
{5, 13, 0, 0, 0, 0, 21, 29} ,
{6, 14, 0, 0, 0, 0, 22, 30} ,
{7, 15, 0, 0, 0, 0, 23, 31} ,
{8, 16, 0, 0, 0, 0, 24, 32}},

    abRemoved{{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0} ,
{0, 0, 0, 0}}
{
    memcpy(anBoard, anStartBoard, sizeof(anStartBoard)); //anBoard = anStartBoard
    memcpy(anTempBoard, anStartBoard, sizeof(anStartBoard)); //anTempBoard = anStartBoard

    QsPiecieFromTo = "";
    nTransferredPiece = 0;

    bTestEnpassant = false;

    bTestPromotion = false;
    QsFuturePromote = "";
    bPromotionConfirmed = false;

    /*anStartBoard = {{1, 9, 0, 0, 0, 0, 17, 25} ,
                    {2, 10, 0, 0, 0, 0, 18, 26} ,
                    {3, 11, 0, 0, 0, 0, 19, 27} ,
                    {4, 12, 0, 0, 0, 0, 20, 28} ,
                    {5, 13, 0, 0, 0, 0, 21, 29} ,
                    {6, 14, 0, 0, 0, 0, 22, 30} ,
                    {7, 15, 0, 0, 0, 0, 23, 31} ,
                    {8, 16, 0, 0, 0, 0, 24, 32}};*/

    float a1_x = 186.5; float a1_y = 75.6; float a1_z = -2.5;
    float a8_x = 331.0; float a8_y = 74.3; float a8_z = -0.3;
    float h1_x = 186.5; float h1_y = -80.7; float h1_z = -4.1;
    float h8_x = 330.3; float h8_y = -77.6; float h8_z = -1.2;
    //                                      "z" to pozycje na styku chwytaka z szachownicą

    for (int digit = 0; digit <= 7; digit++)
    {
        for (int letter = 0; letter <= 7; letter++)
        {
            afChessboardPositions_x[letter][digit] = a1_x +
                    digit*(((a8_x-a1_x)/7)+((letter/14)*(((a1_x-h1_x)/7)-((a8_x-h8_x)/7))))-
                    letter*(((a1_x-h1_x)/7)-((digit/14)*(((h8_x-h1_x)/7)-((a8_x-a1_x)/7))));
            afChessboardPositions_y[letter][digit] = a1_y +
                    digit*(((a8_y-a1_y)/7)+((letter/14)*(((a1_y-h1_y)/7)-((a8_y-h8_y)/7))))-
                    letter*(((a1_y-h1_y)/7)-((digit/14)*(((h8_y-h1_y)/7)-((a8_y-a1_y)/7))));
            afChessboardPositions_z[letter][digit] = a1_z +
                    digit*(((a8_z-a1_z)/7)+((letter/14)*(((a1_z-h1_z)/7)-((a8_z-h8_z)/7))))-
                    letter*(((a1_z-h1_z)/7)-((digit/14)*(((h8_z-h1_z)/7)-((a8_z-a1_z)/7))));
            /*qDebug() << "position " << litery << cyfry << " = " << afChessboardPositions_x[cyfry][litery]
                        << afChessboardPositions_y[cyfry][litery];*/
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
            afRemovedPiecesPositions_x[row][column] = 100 + row*25;
            afRemovedPiecesPositions_y[row][column] = 140 + column*30;
            afRemovedPiecesPositions_z[row][column] = -22.3 - row*((-22.3 + 16.5)/7);
        }
    }
    for (int column = 2; column <= 3; column++)
    {
        for (int row = 0; row <= 7; row++)
        {
            afRemovedPiecesPositions_x[row][column] = 100 + row*25;
            afRemovedPiecesPositions_y[row][column] = -155 - ((column-2)*30);
            afRemovedPiecesPositions_z[row][column] = -22.5 - row*((-22.5 + 17)/7);
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

void Chessboard::findBoardPos(int nFromLetter, int nFromDiggit, int nToLetter, int nToDiggit)
{ //TODO: zrobić z tego strukture
    QsPiecieFromTo = this->findPieceLetterPos(nFromLetter) + nFromDiggit +
            this->findPieceLetterPos(nToLetter) + nToDiggit;
    qDebug() << "QsPiecieFromTo: " << QsPiecieFromTo;

    PieceFrom.Letter = nFromLetter;
    PieceFrom.Digit = nFromDiggit;

    PieceTo.Letter = nToLetter;
    PieceTo.Digit = nToDiggit;
}


void Chessboard::actualPos(int nLetter, int nDigit)
{
    PieceActualPos.Letter = nLetter;
    PieceActualPos.Digit = nDigit;
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
    else qDebug() << "ERROR: Chessboard::findPieceLetterPos(QString QsLetter): Unknown QsLetter value.";

    return nLetter;
}

//TODO: to co robię tutaj poniżej to omijanie czegoś co się zwie template/szablony?
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
    default: qDebug() << "ERROR: Chessboard::findPieceLetterPos(int nLetter): Unknown nLetter value."; break;
    }

    return QsLetter;
}

bool Chessboard::removeStatements() // funkcje do sprawdzania czy bijemy bierkę
{
    if (anBoard[PieceTo.Letter][PieceTo.Digit] != 0) // sprawdzanie czy na pole, gdzie bierka idzie nie jest zajęte
        //TODO: nieprawdziwe dla enpassant!
    {
        //QsPieceToReject = QsPieceTo; //zbijana jest bierka z tego pola na które chcemy iść
        //!!!TODO: TO JEST NIEAKTUALNE
        return 1;
    }
    else return 0;
}

bool Chessboard::castlingStatements() // sprawdzanie czy dany ruch jest prośbą o roszadę
{
    if (PieceFrom.Letter == 4 && (PieceFrom.Digit == 0 || PieceFrom.Digit == 7) //jeżeli ruszana jest bierka z pozycji króla
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

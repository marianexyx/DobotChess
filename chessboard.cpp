#include "chessboard.h"

Chessboard::Chessboard()
{
    nTransferredPiece = 0;

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

    for (int row = 0; row <= 1; row++)
    {
        for (int column = 0; column <= 7; column++)
        {
            afRemovedPiecesPositions_x[row][column] = 100 + column*25;
            afRemovedPiecesPositions_y[row][column] = 140 + row*30;
            afRemovedPiecesPositions_z[row][column] = -22.3 - column*((-22.3 + 16.5)/7);
        }
    }
    for (int row = 2; row <= 3; row++)
    {
        for (int column = 0; column <= 7; column++)
        {
            afRemovedPiecesPositions_x[row][column] = 100 + column*25;
            afRemovedPiecesPositions_y[row][column] = -155 - ((row-2)*30);
            afRemovedPiecesPositions_z[row][column] = -22.5 - column*((-22.5 + 17)/7);
        }
    }
}

void Chessboard::findBoardPos(QString QsPiecePositions)
{
    QsPiecieFromTo = QsPiecePositions.mid(5,4); // e2e4     TODO: czy to jest potrzebne?
    QsPieceFrom = QsPiecePositions.mid(5,2); // e2          TODO: czy to jest potrzebne?
    QsPieceTo   = QsPiecePositions.mid(7,2); // e4          TODO: czy to jest potrzebne?
    qDebug() << "QsPiecieFromTo: " << QsPiecieFromTo;

    PieceFrom.Letter = this->findPieceLetterPos(QsPiecePositions.mid(5,1));
    PieceFrom.Digit = QsPiecePositions.mid(6,1).toInt() - 1;

    PieceTo.Letter = this->findPieceLetterPos(QsPiecePositions.mid(7,1));
    PieceTo.Digit = QsPiecePositions.mid(8,1).toInt() - 1;
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

    return nLetter;
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

bool Chessboard::castlingStatements(QString QsPossibleCastlingVal) // sprawdzanie czy dany ruch jest prośbą o roszadę i czy da się ją zrobić
{
    if (QsPossibleCastlingVal == "e1c1" || QsPossibleCastlingVal == "e1C1" || QsPossibleCastlingVal == "E1c1" || QsPossibleCastlingVal == "E1C1" ||
            QsPossibleCastlingVal == "e1g1" || QsPossibleCastlingVal == "e1G1" || QsPossibleCastlingVal == "E1g1" || QsPossibleCastlingVal == "E1G1" ||
            QsPossibleCastlingVal == "e8c8" || QsPossibleCastlingVal == "e8C8" || QsPossibleCastlingVal == "E8c8" || QsPossibleCastlingVal == "E8C8" ||
            QsPossibleCastlingVal == "e8g8" || QsPossibleCastlingVal == "e8G8" || QsPossibleCastlingVal == "E8g8" || QsPossibleCastlingVal == "E8G8" )
    {
        //ustawiane skąd-dokąd przenoszony będzie król podczas roszady
        //QsKingPosF = "c_" + QsPieceFrom + "K"; //jest to info dla arduino, że mamy do czynienia z roszadą
        //QsKingPosT = "c_" + QsPieceTo + "K";

        //ustawiane skąd-dokąd przenoszona będzie wieża podczas roszady
        if (PieceTo.Letter == 2) //old QsLiteraPolaTo == "c"
        {
            QsRookPosF = "c_[a";
            QsRookPosT = "c_[d";
        }
        else
        {
            QsRookPosF = "c_[h";
            QsRookPosT = "c_[f";
        }
        if (PieceTo.Digit == 1)
        {
            QsRookPosF += "1]fR";
            QsRookPosT += "1]tR";
        }
        else
        {
            QsRookPosF += "8]fR";
            QsRookPosT += "8]tR";
        }
        return 1;
    }
    else
        return 0;
}

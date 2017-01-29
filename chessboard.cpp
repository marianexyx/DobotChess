#include "chessboard.h"

#define ACTUAL_POS  1000;
#define ARM_UP      50;
#define ARM_DOWN    3;

Chessboard::Chessboard()
{
    float a1_x = 186.5; float a1_y = 75.6; float a1_z = -2.5;
    float a8_x = 331.0; float a8_y = 74.3; float a8_z = -0.3;
    float h1_x = 186.5; float h1_y = -80.7; float h1_z = -4.1;
    float h8_x = 330.3; float h8_y = -77.6; float h8_z = -1.2;

    for (int cyfry = 0; cyfry <= 7; cyfry++)
    {
        for (int litery = 0; litery <= 7; litery++)
        {
            afChessboardPositions_x[litery][cyfry] = a1_x +
                    cyfry*(((a8_x-a1_x)/7)+((litery/14)*(((a1_x-h1_x)/7)-((a8_x-h8_x)/7))))-
                    litery*(((a1_x-h1_x)/7)-((cyfry/14)*(((h8_x-h1_x)/7)-((a8_x-a1_x)/7))));
            afChessboardPositions_y[litery][cyfry] = a1_y +
                    cyfry*(((a8_y-a1_y)/7)+((litery/14)*(((a1_y-h1_y)/7)-((a8_y-h8_y)/7))))-
                    litery*(((a1_y-h1_y)/7)-((cyfry/14)*(((h8_y-h1_y)/7)-((a8_y-a1_y)/7))));
            afChessboardPositions_z[litery][cyfry] = a1_z +
                    cyfry*(((a8_z-a1_z)/7)+((litery/14)*(((a1_z-h1_z)/7)-((a8_z-h8_z)/7))))-
                    litery*(((a1_z-h1_z)/7)-((cyfry/14)*(((h8_z-h1_z)/7)-((a8_z-a1_z)/7))));
            /*qDebug() << "position " << litery << cyfry << " = " << afChessboardPositions_x[cyfry][litery]
                        << afChessboardPositions_y[cyfry][litery];*/
        }
    }
}

void Chessboard::findDobotXYZVals(QString QS_msg) //tymczasowaFunkcjaZamieniajacaRuchySzachoweNaWartosciKartezjanskie
{
    qDebug() << "In Chessboard::findDobotXYZVals received: " << QS_msg;
    this->addTextToDobotConsole(QS_msg+"\n");

}

void Chessboard::findBoardPos(QString QsPiecePositions)
{
    QsPiecieFromTo = QsPiecePositions.mid(5,4); // e2e4
    QsPieceFrom = QsPiecePositions.mid(5,2); // e2
    QsPieceTo   = QsPiecePositions.mid(7,2); // e4

    nLiteraPolaFrom = this->findPieceLetterPos(QsPiecePositions.mid(5,1));
    nLiteraPolaTo = this->findPieceLetterPos(QsPiecePositions.mid(7,1));

    nCyfraPolaFrom = QsPiecePositions.mid(6,1).toInt() - 1;
    nCyfraPolaTo = QsPiecePositions.mid(8,1).toInt() - 1;

    PieceFrom.x = afChessboardPositions_x[nLiteraPolaFrom][nCyfraPolaFrom];
    PieceFrom.y = afChessboardPositions_y[nLiteraPolaFrom][nCyfraPolaFrom];
    PieceFrom.z = afChessboardPositions_z[nLiteraPolaFrom][nCyfraPolaFrom];
    PieceFrom.r = ACTUAL_POS;

    PieceTo.x = afChessboardPositions_x[nLiteraPolaTo][nCyfraPolaTo];
    PieceTo.y = afChessboardPositions_y[nLiteraPolaTo][nCyfraPolaTo];
    PieceTo.z = afChessboardPositions_z[nLiteraPolaTo][nCyfraPolaTo];
    PieceTo.r = ACTUAL_POS;

   /*ArmUp.x = ACTUAL_POS;
    ArmUp.y = ACTUAL_POS;
    ArmUp.z = ARM_UP;
    ArmUp.r = ACTUAL_POS;

    ArmDown.x = ACTUAL_POS;
    ArmDown.y = ACTUAL_POS;
    ArmDown.z = ARM_DOWN;
    ArmDown.r = ACTUAL_POS;*/
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
    if (abBoard[nLiteraPolaTo][nCyfraPolaTo] == true) //sprawdzanie czy na pole, gdzie bierka idzie nie jest zajęte (nieprawdziwe dla enpassant)
    {
        QsPieceToReject = QsPieceTo; //zbijana jest bierka z tego pola na które chcemy iść
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
        QsKingPosF = "c_" + QsPieceFrom + "K"; //jest to info dla arduino, że mamy do czynienia z roszadą
        QsKingPosT = "c_" + QsPieceTo + "K";

        //ustawiane skąd-dokąd przenoszona będzie wieża podczas roszady
        if (QsLiteraPolaTo == "c")
        {
            QsRookPosF = "c_[a";
            QsRookPosT = "c_[d";
        }
        else
        {
            QsRookPosF = "c_[h";
            QsRookPosT = "c_[f";
        }
        if(nCyfraPolaTo == 1)
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

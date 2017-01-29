#include "chessboard.h"

#define ACTUAL_POS  1000;
#define ARM_UP      50;
#define ARM_DOWN    3;

Chessboard::Chessboard()
{
    int a1_x = 184.5; int a1_y = 76.3; //int a1_z = -2.1;
    int a8_x = 330.3; int a8_y = 73.1; //int a8_z = 2.2;
    int h1_x = 184.5; int h1_y = -81.8; //int h1_z = -2.7;
    int h8_x = 329.2; int h8_y = -78.5; //int h8_z = 1.6;

    for (int cyfry = 0; cyfry <= 7; cyfry++)
    {
        for (int litery = 0; litery <= 7; litery++)
        {
            afChessboardPositions_x[cyfry][litery] = a1_x +
                    cyfry*(((a8_x-a1_x)/7)+((litery/14)*(((a1_x-h1_x)/7)-((a8_x-h8_x)/7))))-
                    litery*(((a1_x-h1_x)/7)-((cyfry/14)*(((h8_x-h1_x)/7)-((a8_x-a1_x)/7))));
            afChessboardPositions_y[cyfry][litery] = a1_y +
                    cyfry*(((a8_y-a1_y)/7)+((litery/14)*(((a1_y-h1_y)/7)-((a8_y-h8_y)/7))))-
                    litery*(((a1_y-h1_y)/7)-((cyfry/14)*(((h8_y-h1_y)/7)-((a8_y-a1_y)/7))));
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

    nCyfraPolaFrom = QsPiecePositions.mid(6,1).toInt() - 1;
    nCyfraPolaTo = QsPiecePositions.mid(8,1).toInt() - 1;

    QsLiteraPolaFrom = QsPiecePositions.mid(5,1);
    QsLiteraPolaTo = QsPiecePositions.mid(7,1);

    if (QsLiteraPolaFrom == "a" || QsLiteraPolaFrom == "A") {nLiteraPolaFrom = 0; QsLiteraPolaFrom = "a";}
    else if (QsLiteraPolaFrom == "b" || QsLiteraPolaFrom == "B") {nLiteraPolaFrom = 1; QsLiteraPolaFrom = "b";}
    else if (QsLiteraPolaFrom == "c" || QsLiteraPolaFrom == "C") {nLiteraPolaFrom = 2; QsLiteraPolaFrom = "c";}
    else if (QsLiteraPolaFrom == "d" || QsLiteraPolaFrom == "D") {nLiteraPolaFrom = 3; QsLiteraPolaFrom = "d";}
    else if (QsLiteraPolaFrom == "e" || QsLiteraPolaFrom == "E") {nLiteraPolaFrom = 4; QsLiteraPolaFrom = "e";}
    else if (QsLiteraPolaFrom == "f" || QsLiteraPolaFrom == "F") {nLiteraPolaFrom = 5; QsLiteraPolaFrom = "f";}
    else if (QsLiteraPolaFrom == "g" || QsLiteraPolaFrom == "G") {nLiteraPolaFrom = 6; QsLiteraPolaFrom = "g";}
    else if (QsLiteraPolaFrom == "h" || QsLiteraPolaFrom == "H") {nLiteraPolaFrom = 7; QsLiteraPolaFrom = "h";}

    if (QsLiteraPolaTo== "a" || QsLiteraPolaTo== "A") {nLiteraPolaTo = 0; QsLiteraPolaTo= "a";}
    else if (QsLiteraPolaTo== "b" || QsLiteraPolaTo== "B") {nLiteraPolaTo = 1; QsLiteraPolaTo= "b";}
    else if (QsLiteraPolaTo== "c" || QsLiteraPolaTo== "C") {nLiteraPolaTo = 2; QsLiteraPolaTo= "c";}
    else if (QsLiteraPolaTo== "d" || QsLiteraPolaTo== "D") {nLiteraPolaTo = 3; QsLiteraPolaTo= "d";}
    else if (QsLiteraPolaTo== "e" || QsLiteraPolaTo== "E") {nLiteraPolaTo = 4; QsLiteraPolaTo= "e";}
    else if (QsLiteraPolaTo== "f" || QsLiteraPolaTo== "F") {nLiteraPolaTo = 5; QsLiteraPolaTo= "f";}
    else if (QsLiteraPolaTo== "g" || QsLiteraPolaTo== "G") {nLiteraPolaTo = 6; QsLiteraPolaTo= "g";}
    else if (QsLiteraPolaTo== "h" || QsLiteraPolaTo== "H") {nLiteraPolaTo = 7; QsLiteraPolaTo= "h";}

    PieceFrom.x = afChessboardPositions_x[nCyfraPolaFrom][nLiteraPolaFrom];
    PieceFrom.y = afChessboardPositions_y[nCyfraPolaFrom][nLiteraPolaFrom];
    PieceFrom.z = ACTUAL_POS;
    PieceFrom.r = ACTUAL_POS;

    PieceTo.x = afChessboardPositions_x[nCyfraPolaTo][nLiteraPolaTo];
    PieceTo.y = afChessboardPositions_y[nCyfraPolaTo][nLiteraPolaTo];
    PieceTo.z = ACTUAL_POS;
    PieceTo.r = ACTUAL_POS;

    ArmUp.x = ACTUAL_POS;
    ArmUp.y = ACTUAL_POS;
    ArmUp.z = ARM_UP;
    ArmUp.r = ACTUAL_POS;

    ArmDown.x = ACTUAL_POS;
    ArmDown.y = ACTUAL_POS;
    ArmDown.z = ARM_DOWN;
    ArmDown.r = ACTUAL_POS;
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

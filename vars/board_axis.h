#ifndef BOARD_AXIS_H
#define BOARD_AXIS_H

#pragma once
#include <QString>

enum LETTER
{
    L_X = 0,
    L_A = 1,
    L_B = 2,
    L_C = 3,
    L_D = 4,
    L_E = 5,
    L_F = 6,
    L_G = 7,
    L_H = 8
};

enum DIGIT
{
    D_X = 0,
    D_1 = 1,
    D_2 = 2,
    D_3 = 3,
    D_4 = 4,
    D_5 = 5,
    D_6 = 6,
    D_7 = 7,
    D_8 = 8
};

inline LETTER pieceLetterPos(QString QStrLetter)
{
    if (QStrLetter == "a" || QStrLetter == "A") return L_A;
    else if (QStrLetter == "b" || QStrLetter == "B") return L_B;
    else if (QStrLetter == "c" || QStrLetter == "C") return L_C;
    else if (QStrLetter == "d" || QStrLetter == "D") return L_D;
    else if (QStrLetter == "e" || QStrLetter == "E") return L_E;
    else if (QStrLetter == "f" || QStrLetter == "F") return L_F;
    else if (QStrLetter == "g" || QStrLetter == "G") return L_G;
    else if (QStrLetter == "h" || QStrLetter == "H") return L_H;
    else
    {
        qDebug() << "ERROR: pieceLetterPos(QString QStrLetter): "
                     "Unknown QStrLetter value =" << QStrLetter;
        return L_X;
    }
}

inline QString pieceLetterPosAsQStr(LETTER Letter)
{
    switch(Letter)
    {
    case L_X: return "-1";
    case L_A: return "a";
    case L_B: return "b";
    case L_C: return "c";
    case L_D: return "d";
    case L_E: return "e";
    case L_F: return "f";
    case L_G: return "g";
    case L_H: return "h";
    default:
        qDebug() << "ERROR: pieceLetterPosAsQStr(LETTER letter): "
                    "Unknown letter value =" << Letter;
        return "";
    }
}

#endif // BOARD_AXIS_H

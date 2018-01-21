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
    LETTER letter = L_X;

    if (QStrLetter == "a" || QStrLetter == "A") {letter = L_A; }
    else if (QStrLetter == "b" || QStrLetter == "B") {letter = L_B;}
    else if (QStrLetter == "c" || QStrLetter == "C") {letter = L_C;}
    else if (QStrLetter == "d" || QStrLetter == "D") {letter = L_D;}
    else if (QStrLetter == "e" || QStrLetter == "E") {letter = L_E;}
    else if (QStrLetter == "f" || QStrLetter == "F") {letter = L_F;}
    else if (QStrLetter == "g" || QStrLetter == "G") {letter = L_G;}
    else if (QStrLetter == "h" || QStrLetter == "H") {letter = L_H;}
    else qDebug() << "ERROR: pieceLetterPos(QString QStrLetter): "
                     "Unknown QStrLetter value =" << QStrLetter;

    return letter;
}

inline QString pieceLetterPosAsQStr(LETTER letter)
{
    QString QStrLetter;

    switch(letter)
    {
    case L_X: QStrLetter = "-1"; break;
    case L_A: QStrLetter = "a"; break;
    case L_B: QStrLetter = "b"; break;
    case L_C: QStrLetter = "c"; break;
    case L_D: QStrLetter = "d"; break;
    case L_E: QStrLetter = "e"; break;
    case L_F: QStrLetter = "f"; break;
    case L_G: QStrLetter = "g"; break;
    case L_H: QStrLetter = "h"; break;
    default:
        qDebug() << "ERROR: pieceLetterPosAsQStr(LETTER letter): "
                    "Unknown letter value =" << letter;
        break;
    }

    return QStrLetter;
}

#endif // BOARD_AXIS_H

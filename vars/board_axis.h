#ifndef BOARD_AXIS_H
#define BOARD_AXIS_H

#pragma once
#include <QString>

enum LETTER
{
    L_X = -1,
    L_A = 0,
    L_B = 1,
    L_C = 2,
    L_D = 3,
    L_E = 4,
    L_F = 5,
    L_G = 6,
    L_H = 7
};

enum DIGIT
{
    D_X = -1,
    D_1 = 0,
    D_2 = 1,
    D_3 = 2,
    D_4 = 3,
    D_5 = 4,
    D_6 = 5,
    D_7 = 6,
    D_8 = 7
};

inline LETTER pieceLetterPos(QString QStrLetter)
{
    LETTER letter;

    if (QStrLetter == "a" || QStrLetter == "A") {letter = L_A; }
    else if (QStrLetter == "b" || QStrLetter == "B") {letter = L_B;}
    else if (QStrLetter == "c" || QStrLetter == "C") {letter = L_C;}
    else if (QStrLetter == "d" || QStrLetter == "D") {letter = L_D;}
    else if (QStrLetter == "e" || QStrLetter == "E") {letter = L_E;}
    else if (QStrLetter == "f" || QStrLetter == "F") {letter = L_F;}
    else if (QStrLetter == "g" || QStrLetter == "G") {letter = L_G;}
    else if (QStrLetter == "h" || QStrLetter == "H") {letter = L_H;}
    else qDebug() << "ERROR: pieceLetterPos(QString QStrLetter): "
                     "Unknown QStrLetter value.";

    return letter;
}

inline QString pieceLetterPos(LETTER letter)
{
    QString QStrLetter;

    switch(letter)
    {
    case L_A: QStrLetter = "a"; break;
    case L_B: QStrLetter = "b"; break;
    case L_C: QStrLetter = "c"; break;
    case L_D: QStrLetter = "d"; break;
    case L_E: QStrLetter = "e"; break;
    case L_F: QStrLetter = "f"; break;
    case L_G: QStrLetter = "g"; break;
    case L_H: QStrLetter = "h"; break;
    default:
        qDebug() << "ERROR: pieceLetterPos(LETTER letter): Unknown letter value.";
        break;
    }

    return QStrLetter;
}

#endif // BOARD_AXIS_H

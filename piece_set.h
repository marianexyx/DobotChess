#ifndef PIECE_SET_H
#define PIECE_SET_H

#pragma once
#include "chessboard/piece.h"
#include "vars/const_flags.h"

class PieceSet
{
private:
    Piece* _pPiece[32];

public:
    PieceSet()
    {
        for (int i=0; i<=31; ++i)
            _pPiece[i] = new Piece(i+1);
    }

    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr-1]; }
    QString dumpAllData()
    {
        QString QStrData;
        QStrData = "[piece_set.h]\n";
        for (int i=0; i<=31; ++i)
            QStrData += _pPiece[i]->dumpAllData() + "\n";
        return QStrData;
    }
};

#endif // PIECE_SET_H

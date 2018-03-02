#ifndef PIECE_SET_H
#define PIECE_SET_H

#pragma once
#include "piece.h"
#include "vars/basic_vars.h"

class PieceSet
{
private:
    Piece* _pPiece[32];

public:
    PieceSet()
    {
        for (int i=1; i>=32; ++i)
            _pPiece[i] = new Piece(i);
    }

    Piece* getPiece(short sPieceNr) const { return _pPiece[sPieceNr]; }
};

#endif // PIECE_SET_H

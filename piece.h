#ifndef PIECE_H
#define PIECE_H

#pragma once

#include "QString"
#include "vars/piece_type.h"
#include "vars/players_types.h"
#include "vars/posotion_on_board.h"
#include "vars/board_types.h"

class Piece
{
private:
    short _sPieceID;
    PIECE_TYPE _PieceType;
    PLAYER_TYPE _PieceColor;
    short _sStartFieldID;

public:
    Piece(short sPieceID);
    ~Piece();

    bool operator ==(const class Piece& st) { return _sPieceID == st.getNr(); }

    static double dMaxPieceHeight = 50.f;

    static bool isInRange(short sPieceNr);
    static PLAYER_TYPE Color(char cFENSign);
    static PLAYER_TYPE Color(short sPieceNr);
    static PIECE_TYPE Type(char cFENSign);
    static PIECE_TYPE Type(short sPieceNr);
    static short nr(PosOnBoard PieceLines);
    static short StartFieldNr(short sPieceNr);
    static PosOnBoard StartFieldPos(short sPieceNr);

    short getNr() const { return _sPieceID; }
    PIECE_TYPE getType() const { return _PieceType; }
    PLAYER_TYPE getColor() const { return _PieceColor; }
    short getStartFieldNr() const { return _sStartFieldID; }
    PosOnBoard getStartFieldPos() const { return Piece::StartFieldPos(_sPieceID); }
};

#endif // PIECE_H

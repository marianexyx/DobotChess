#ifndef PIECE_H
#define PIECE_H

#pragma once

#include "QDebug"
#include "QString"
#include "vars/players_types.h"
#include "vars/posotion_on_board.h"

enum PIECE_TYPE
{
    P_PAWN = 112, //p
    P_ROOK = 114, //r
    P_KNIGHT = 110, //n
    P_BISHOP = 98, //b
    P_KING = 107, //k
    P_QUEEN = 113 //q
};

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

    bool operator ==(const class Piece& p) { return _sPieceID == p.getNr(); }

    static double dMaxPieceHeight = 50.f;

    static bool isInRange(short sPieceNr);
    static PLAYER_TYPE Color(char cFENSign);
    static PLAYER_TYPE Color(short sPieceNr);
    static PIECE_TYPE Type(char cFENSign);
    static PIECE_TYPE Type(short sPieceNr);
    static QString name(short sPieceNr);
    static short nr(PosOnBoard PieceLines);
    static short startFieldNr(short sPieceNr);
    static PosOnBoard StartFieldPos(short sPieceNr);

    short getNr() const { return _sPieceID; }
    PIECE_TYPE getType() const { return _PieceType; }
    PLAYER_TYPE getColor() const { return _PieceColor; }
    QString getName() const { return Piece::name(_sPieceID); }
    short getStartFieldNr() const { return _sStartFieldID; }
    PosOnBoard getStartFieldPos() const { return Piece::StartFieldPos(_sPieceID); }
};

#endif // PIECE_H

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
    short m_sPieceID;
    PIECE_TYPE m_PieceType;
    PLAYER_TYPE m_PieceColor;
    short m_sStartFieldID;

public:
    Piece(short sPieceID); //todo: friend dla chessboard
    ~Piece(); //todo: friend dla chessboard

    static double dMaxPieceHeight = 50.f;

    static bool isInRange(short sPieceNr);
    static PLAYER_TYPE Color(char cFENSign);
    static PLAYER_TYPE Color(short sPieceNr);
    static PIECE_TYPE Type(char cFENSign);
    static PIECE_TYPE Type(short sPieceNr);
    static short nr(PositionOnBoard PieceLines);
    static short StartFieldNr(short sPieceNr);
    static PositionOnBoard StartFieldPos(short sPieceNr);

    short getNr() const { return m_sPieceID; }
    PIECE_TYPE getType() const { return m_PieceType; }
    PLAYER_TYPE getColor() const { return m_PieceColor; }
    short getStartFieldNr() const { return m_sStartFieldID; }
    PositionOnBoard getStartFieldPos() const { return Piece::StartFieldPos(m_sPieceID); }
};

#endif // PIECE_H

#ifndef PIECE_H
#define PIECE_H

#pragma once
#include "QDebug"
#include "QString"
#include "vars/players_types.h"
#include "chessboard/vars/position_on_board.h"

enum PIECE_TYPE
{
    P_ERROR = 'e',
    P_PAWN = 'p',
    P_ROOK = 'r',
    P_KNIGHT = 'n',
    P_BISHOP = 'b',
    P_KING = 'k',
    P_QUEEN = 'q'
};

class Piece
{
private:
    short _sPieceID;
    PIECE_TYPE _pieceType;
    PLAYER_TYPE _pieceColor;
    short _sStartFieldID;
    PIECE_TYPE _promotedType;

public:
    Piece(short sPieceID);

    bool operator ==(const class Piece& p) { return _sPieceID == p.getNr(); }

    static bool isInRange(short sPieceNr);
    static PLAYER_TYPE Color(char chFENSign);
    static PLAYER_TYPE Color(short sPieceNr);
    static PIECE_TYPE Type(char chFENSign);
    static PIECE_TYPE Type(QString QStrFENSign);
    static PIECE_TYPE Type(short sPieceNr);
    static short nr(PosOnBoard pieceLines);
    static short startFieldNr(short sPieceNr);
    static PosOnBoard startFieldPos(short sPieceNr);

    void setPromotedType(PIECE_TYPE promotedType);
    void clearPromotedType() { _promotedType = P_ERROR; }

    short getNr() const { return _sPieceID; }
    QString getAsFENSign();
    PIECE_TYPE getType() const { return _pieceType; }
    PLAYER_TYPE getColor() const { return _pieceColor; }
    QString getName();
    short getStartFieldNr() const { return _sStartFieldID; }
    PosOnBoard getStartFieldPos() const { return Piece::startFieldPos(_sPieceID); }
    PIECE_TYPE getPromotedType() const { return _promotedType; }
};

#endif // PIECE_H

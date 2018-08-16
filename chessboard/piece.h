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
    ushort m_usPieceID;
    PIECE_TYPE m_pieceType;
    PLAYER_TYPE m_PieceColor;
    ushort m_usStartFieldID;
    PIECE_TYPE m_PromotedType;

public:
    Piece(ushort usPieceID);

    bool operator ==(const class Piece& p) { return m_usPieceID == p.getNr(); }

    static bool isInRange(ushort usPieceNr);
    static PLAYER_TYPE Color(char chFENSign);
    static PLAYER_TYPE Color(ushort usPieceNr);
    static PIECE_TYPE Type(char chFENSign);
    static PIECE_TYPE Type(QString QStrFENSign);
    static PIECE_TYPE Type(ushort usPieceNr);
    static ushort nr(PosOnBoard pieceLines);
    static ushort startFieldNr(ushort usPieceNr);
    static PosOnBoard startFieldPos(short sPieceNr);

    void setPromotedType(PIECE_TYPE promotedType);
    void clearPromotedType() { m_PromotedType = P_ERROR; }

    ushort getNr() const { return m_usPieceID; }
    PIECE_TYPE getType() const { return m_pieceType; }
    PLAYER_TYPE getColor() const { return m_PieceColor; }
    ushort getStartFieldNr() const { return m_usStartFieldID; }
    PosOnBoard getStartFieldPos() const { return Piece::startFieldPos(m_usPieceID); }
    PIECE_TYPE getPromotedType() const { return m_PromotedType; }
    QString getName();
    QString getAsFENSign();
    QString dumpAllData();
};

#endif // PIECE_H

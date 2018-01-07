#ifndef SEQUENCE_TYPES_H
#define SEQUENCE_TYPES_H

#pragma once
#include <QString>

//todo: kilka z tych nie jest używana w findMoveType
enum SEQUENCE_TYPE { ST_NONE, //todo: trzeba to będzie podzielić na ruchy szachowe i ramienia
                     ST_REGULAR,
                     ST_PROMOTE_TO_WHAT,
                     ST_PROMOTION,
                     ST_ENPASSANT,
                     ST_CASTLING,
                     ST_CASTLING_KING,
                     ST_CASTLING_ROOK,
                     ST_REMOVING,
                     ST_RESTORE, //restore available only in reset command
                     ST_SERVICE };

inline QString sequenceTypeAsQstr(SEQUENCE_TYPE ST)
{
    QString QStrType;
    switch(ST)
    {
    case ST_NONE: QStrType = "none" ; break;
    case ST_REGULAR: QStrType = "regular" ; break;
    case ST_PROMOTE_TO_WHAT: QStrType = "promoteToWhat" ; break;
    case ST_PROMOTION: QStrType = "promotion" ; break;
    case ST_ENPASSANT: QStrType = "enpassant" ; break;
    case ST_CASTLING: QStrType = "castling" ; break;
    case ST_CASTLING_KING: QStrType = "castlingKing" ; break;
    case ST_CASTLING_ROOK: QStrType = "castlingRook" ; break;
    case ST_REMOVING: QStrType = "removing" ; break;
    case ST_RESTORE: QStrType = "restore" ; break;
    case ST_SERVICE: QStrType = "service" ; break;
    default:
        QStrType = "ERROR: wrong sequenceTypeAsQstr arg = " + ST;
        qDebug() << QStrType;
        break;
    }

    return QStrType;
}

#endif // SEQUENCE_TYPES_H

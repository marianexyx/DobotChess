#ifndef SEQUENCE_TYPES_H
#define SEQUENCE_TYPES_H

#pragma once
#include <QString>
#include <QDebug>

enum SEQUENCE_TYPE
{
    ST_NONE,
    ST_REGULAR,
    ST_PROMOTE_TO_WHAT,
    ST_PROMOTION,
    ST_ENPASSANT,
    ST_CASTLING,
    ST_REMOVING
};

inline QString sequenceTypeAsQstr(SEQUENCE_TYPE ST)
{
    switch(ST)
    {
    case ST_NONE: return "none";
    case ST_REGULAR: return "regular";
    case ST_PROMOTE_TO_WHAT: return "promoteToWhat";
    case ST_PROMOTION: return "promotion";
    case ST_ENPASSANT: return "enpassant";
    case ST_CASTLING: return "castling";
    case ST_REMOVING: return "removing";
    default:
        qDebug() << "ERROR: sequenceTypeAsQstr(): wrong arg =" << ST;
        return "";
    }
}

#endif // SEQUENCE_TYPES_H

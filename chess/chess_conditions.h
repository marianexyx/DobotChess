#ifndef CHESS_CONDITIONS_H
#define CHESS_CONDITIONS_H

#pragma once
#include "chess.h"
#include "chess/request_types.h"

//future: wszystkie errory zapisywać w plikach z logami, z oznaczeniami kto, co, kiedy...
//...wysyłał i w jakich okolicznościach

class ChessConditions
{
    friend class Chess;

private:
    Chess* _pChess;

    bool isRequestParameterInProperFormat(clientRequest request);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type);
    bool isSenderAppropriate(Client* pSender, REQUEST_TYPE Type);
    bool isThereAnySpecialConditionBeenMet(Client* pSender, clientRequest request);

//public:
    ChessConditions(Chess* pChess);

    bool isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender);
    QString extractParameter(REQUEST_TYPE Type, QString QStrRequest);
};

#endif // CHESS_CONDITIONS_H

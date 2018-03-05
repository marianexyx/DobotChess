#ifndef CHESS_CONDITIONS_H
#define CHESS_CONDITIONS_H

#pragma once
#include "chess/vars/request_types.h"
#include "chess/vars/game_status.h"
#include "client.h"
#include "chess/chess_status.h"

//future: wszystkie errory zapisywać w plikach z logami, z oznaczeniami kto, co, kiedy...
//...wysyłał i w jakich okolicznościach

class ChessConditions
{
    friend class Chess;

private:
    Clients* _pClientsList;
    ChessStatus* _pStatus;

    bool isRequestParameterInProperFormat(clientRequest request);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type, GAME_STATUS Status);
    bool isSenderAppropriate(Client& sender, REQUEST_TYPE Type);
    bool isThereAnySpecialConditionBeenMet(Client& sender, clientRequest request);

//public:
    ChessConditions(Clients* pClientsList, ChessStatus* pStatus);

    bool isClientRequestCanBeAccepted(QString QStrMsg, Client& sender);
    QString extractParameter(REQUEST_TYPE Type, QString QStrRequest);
};

#endif // CHESS_CONDITIONS_H

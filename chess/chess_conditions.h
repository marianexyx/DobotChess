#ifndef CHESS_CONDITIONS_H
#define CHESS_CONDITIONS_H

#pragma once
#include "chess/vars/request_types.h"
#include "chess/vars/game_status.h"
#include "client.h"
#include "chess/chess_status.h"
#include "sql.h"

class ChessConditions
{
    friend class Chess;

private:
    Clients* _pClientsList;
    ChessStatus* _pStatus;

    bool isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender,
                                      GAME_STATUS GS, REJECTED_REQUEST_REACTION& RRR);
    QString extractParameterIfTypeIsInProperFormat(REQUEST_TYPE Type, QString QStrMsg,
                                                   REJECTED_REQUEST_REACTION& RRR);
    bool isRequestAParameterType(REQUEST_TYPE Type, bool bErrorLog = false);
    bool isRequestParameterInProperFormat(clientRequest request);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type, GAME_STATUS Status);
    bool isSenderAppropriate(Client *pSender, REQUEST_TYPE Type);
    bool isThereAnySpecialConditionBeenMet(Client *pSender, clientRequest request,
                                           REJECTED_REQUEST_REACTION& RRR);

    ChessConditions(Clients* pClientsList, ChessStatus* pStatus);
};

#endif // CHESS_CONDITIONS_H

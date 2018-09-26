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
    Clients* m_pClientsList;
    ChessStatus* m_pStatus;

    ChessConditions(Clients* pClientsList, ChessStatus* pStatus);

    static bool isRequestMsgInProperFormat(QString QStrMsg);
    static bool isRequestAParameterType(REQUEST_TYPE Type, bool bErrorLog = false);
    static bool isRequestParameterInProperFormat(clientRequest request);
    static QString extractParamIfExists(QString QStrMsg);
    REJECTED_REQUEST_REACTION isClientRequestCanBeExecuted(clientRequest request, GAME_STATUS GS);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type, GAME_STATUS Status);
    bool isSenderAppropriate(Client *pSender, REQUEST_TYPE Type);
    REJECTED_REQUEST_REACTION isThereAnySpecialConditionBeenMet(Client *pSender,
                                                                clientRequest request);
};

#endif // CHESS_CONDITIONS_H

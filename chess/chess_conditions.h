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

    QString extractParameterIfTypeIsInProperFormat(REQUEST_TYPE Type, QString QStrMsg);
    bool isRequestAParameterType(REQUEST_TYPE Type, bool bErrorLog = false);
    bool isRequestParameterInProperFormat(clientRequest request);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type, GAME_STATUS Status);
    bool isSenderAppropriate(Client *pSender, REQUEST_TYPE Type);
    bool isThereAnySpecialConditionBeenMet(Client *pSender, clientRequest request);

    //todo: jak publiczność jest załatwiana pomiędzy klasami chess? tak jak tu wszędzie? wydaje...
    //...się ttuaj że to działa
//public:
    ChessConditions(Clients* pClientsList, ChessStatus* pStatus);

    bool isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender,
                                      GAME_STATUS GS);
};

#endif // CHESS_CONDITIONS_H

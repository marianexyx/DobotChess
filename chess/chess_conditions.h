#ifndef CHESS_CONDITIONS_H
#define CHESS_CONDITIONS_H

#pragma once
#include "chess.h"
#include "vars/request_types.h"

struct clientRequest
{
    REQUEST_TYPE type;
    QString parameter;

    clientRequest() { request = RT_NONE; parameter.clear(); }
    clientRequest(REQUEST_TYPE req, QString par) { request = req; parameter = par; }
    void clear() { request = RT_NONE; parameter.clear(); }
};

class ChessConditions
{
private:
    Chess* _pChess;

    bool isRequestParameterInProperFormat(clientRequest request);
    bool isRequestAppropriateToGameStatus(REQUEST_TYPE Type);
    bool isSenderAppropriate(Client* pSender, REQUEST_TYPE Type);
    bool isThereAnySpecialConditionBeenMet(Client* pSender, clientRequest request);

public:
    ChessConditions(Chess* pChess);

    bool isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender);
    QString extractParameter(REQUEST_TYPE Type, QString QStrRequest);

    clientRequest getClientRequest() const { return _request; }
};

#endif // CHESS_CONDITIONS_H

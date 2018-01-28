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

    clientRequest _request;

    bool isRequestParameterInProperFormat();

public:
    ChessConditions(Chess* pChess);
    ~ChessConditions();

    //todo: private- które?
    bool isMsgFromClientCanBeAccepted(QString QStrMsg);
    bool isRequestAppropriateToGameStatus(QString QStrMsg, bool bErrorLog = false);
    bool isSenderAppropriate(QString QStrMsg, bool bErrorLog = false);
    bool isThereAnySpecialConditionBeenMet(QString QStrMsg, bool bErrorLog = false);

    QString extractParameter(QString QStrRequest);

    clientRequest getClientRequest() const { return _request; }
};

#endif // CHESS_CONDITIONS_H

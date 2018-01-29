#include "chess_conditions.h"

ChessConditions::ChessConditions(Chess* pChess)
{
    _pChess = pChess;
}

bool ChessConditions::isClientRequestCanBeAccepted(QString QStrMsg, Client* pSender)
{
    clientRequest request;

    if (requestType(QStrMsg, SHOW_ERRORS) == RT_NONE) return false;
    request.type = requestType(QStrMsg, SHOW_ERRORS);
    request.parameter = this->extractParameter(request.type, QStrMsg);

    if (!this->isRequestParameterInProperFormat(request)) return false;
    if (!this->isRequestAppropriateToGameStatus(request.type)) return false;
    if (!this->isSenderAppropriate(pSender, request.type)) return false;
    if (!this->isThereAnySpecialConditionBeenMet(pSender, request)) return false;

    //todo: przejechać się po wszystkich tych warunkach z innego kodu i zobaczyć czy...
    //...czegoś nie pominąłem. zrobić to podczas jego usuwania
    return true;
}

bool ChessConditions::isRequestParameterInProperFormat(clientRequest request)
{
    ChessMovements* pMovements = _pChess->getMovementsPointer();

    switch(request.type)
    {
    case RT_MOVE:
    {
        if (pMovements->findMoveType(request.parameter) == ST_NONE ||
                pMovements->findMoveType(request.parameter) == ST_PROMOTE_TO_WHAT)
            return false;
        else return true;
    }
    case RT_SIT_ON:
    {
        if (playerTypeFromQStr(request.parameter) != PT_WHITE &&
                playerTypeFromQStr(request.parameter) != PT_BLACK)
            return false;
        else return true;
    }
    case RT_IM:
    {
        if (request.parameter.isEmpty())
            return false;
        else return true;
    }
    case RT_PROMOTE_TO:
    {
        if (ChessStatus::isSignProperPromotionType(request.parameter))
            return true;
        else return false;
    }
    default:
        qDebug() << "ERROR: ChessConditions::isRequestParameterInProperFormat(): unknown "
                    "_request.type:" << requestTypeAsQStr(request.type);
        return false;
    }
}

QString ChessConditions::extractParameter(REQUEST_TYPE Type, QString QStrRequest)
{
    if (Type != RT_MOVE && Type != RT_SIT_ON && Type != RT_IM && Type != RT_PROMOTE_TO)
    {
        qDebug() << "ERROR: ChessConditions::extractParameter(): this kind of request.type"
                    " can't contain a parameter:" << requestTypeAsQStr(Type);
        return "";
    }

    return QStrRequest.mid(requestTypeAsQStr(Type).length() + 1);
}


bool ChessConditions::isRequestAppropriateToGameStatus(REQUEST_TYPE Type)
{
    GAME_STATUS Status = _pChess->getGameStatus();

    switch(Type)
    {
    case RT_NONE:
    {
        qDebug() << "ERROR: ChessConditions::isRequestAppropriateToGameStatus(): Type = RT_NONE";
        return false;
    }
    case RT_NEW_GAME: return Status == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS ? true : false;
    case RT_MOVE: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_GIVE_UP: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_SIT_ON: return Status == GS_TURN_NONE_WAITING_FOR_PLAYERS ? true : false;
    case RT_STAND_UP: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_PROMOTE_TO:
    {
        if (Status == GS_TURN_WHITE_PROMOTE || Status == GS_TURN_BLACK_PROMOTE) return true;
        else return false;
    }
    case RT_QUEUE_ME: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    default: return true;
    }
}

bool ChessConditions::isSenderAppropriate(Client* pSender, REQUEST_TYPE Type)
{
    Clients* pClients = _pChess->getClientsPointer();

    if (!pClients->isClientInList(pSender)) return false;

    bool bLogged = pClients->isClientLoggedIn(pSender);
    bool bSittingOnChair = pClients->isClientAPlayer(pSender);
    bool bInQueue = pClients->isClientInQueue(pSender);

    switch(Type)
    {
    case RT_NONE:
    {
        qDebug() << "ERROR: ChessConditions::isSenderAppropriate(): Type = RT_NONE";
        return false;
    }
    case RT_NEW_GAME:
    case RT_MOVE:
    case RT_GIVE_UP:
    case RT_STAND_UP:
    case RT_PROMOTE_TO:
    {
        if (bSittingOnChair && !bInQueue) return true;
        else return false;
    }
    case RT_SIT_ON:
    case RT_IM:
    case RT_QUEUE_ME:
    {
        if (bLogged && !bSittingOnChair && !bInQueue) return true;
        else return false;
    }
    case RT_LEAVE_QUEUE:
    {
        if (bLogged && !bSittingOnChair && bInQueue) return true;
        else return false;
    }
    default: return true; //case getTableDataAsJSON
    }
}

bool ChessConditions::isThereAnySpecialConditionBeenMet(Client* pSender, clientRequest request)
{
    Clients* pClients = _pChess->getClientsPointer();
    ChessStatus* pStatus = _pChess->getStatusPointer();

    switch(request.type)
    {
    case RT_NONE:
    {
        qDebug() << "ERROR: ChessConditions::isSenderAppropriate(): Type = RT_NONE";
        return false;
    }
    case RT_MOVE:
    case RT_PROMOTE_TO:
    {
        if ((pClients->getClientType(pSender) == PT_WHITE
             && pStatus->getWhoseTurn() == WHITE_TURN) ||
                (pClients->getClientType(pSender) == PT_BLACK
                 && pStatus->getWhoseTurn() == BLACK_TURN))
            return true;
        else return false;
    }
    case RT_SIT_ON:
    {
        PLAYER_TYPE PlayerChair = playerTypeFromQStr(request.parameter);
        if (pClients->isPlayerChairEmpty(PlayerChair, SHOW_ERRORS) &&
                !pClients->isClientAPlayer(pSender, SHOW_ERRORS))
            return true;
        else return false;
    }
    case RT_IM: //todo: double login obłusigwać poza ChessConditions
    {
        if (pSender->name.isEmpty() && !pClients->isClientNameExists(pSender->name, SHOW_ERRORS))
            return true;
        else return false;
    }
    case RT_QUEUE_ME:
    {
        if (pClients->isGameTableOccupied())
            return true;
        else return false;
    }
    case RT_LEAVE_QUEUE:
    {
        if (pClients->isClientInQueue(pSender))
            return true;
        else return false;
    }
    default: return true;
    }
}


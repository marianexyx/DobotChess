#include "chess_conditions.h"

ChessConditions::ChessConditions(Clients* pClientsList)
{
    _pClientsList = pClientsList;
}

bool ChessConditions::isClientRequestCanBeAccepted(QString QStrMsg, Client& sender)
{
    clientRequest r;

    if (requestType(QStrMsg, SHOW_ERRORS) == RT_NONE) return false;
    r.type = requestType(QStrMsg, SHOW_ERRORS);

    if (r.type == RT_MOVE || r.type == RT_SIT_ON || r.type == RT_IM || r.type == RT_PROMOTE_TO)
        r.param = this->extractParameter(r.type, QStrMsg);

    if (!this->isRequestParameterInProperFormat(r)) return false;
    if (!this->isRequestAppropriateToGameStatus(r.type)) return false;
    if (!this->isSenderAppropriate(sender, r.type)) return false;
    if (!this->isThereAnySpecialConditionBeenMet(sender, r)) return false;

    return true;
}

bool ChessConditions::isRequestParameterInProperFormat(clientRequest request)
{
    switch(request.type)
    {
    case RT_MOVE:
        if (_pMovements->findMoveType(request.param) == ST_NONE ||
                _pMovements->findMoveType(request.param) == ST_PROMOTE_TO_WHAT)
            return false;
        else return true;
    case RT_SIT_ON:
        if (playerTypeFromQStr(request.param) != PT_WHITE &&
                playerTypeFromQStr(request.param) != PT_BLACK)
            return false;
        else return true;
    case RT_IM:
        if (request.param.isEmpty())
            return false;
        else return true;
    case RT_PROMOTE_TO:
        if (ChessStatus::isSignProperPromotionType(request.param))
            return true;
        else return false;
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
        qDebug() << "ERROR: ChessConditions::isRequestAppropriateToGameStatus(): Type = RT_NONE";
        return false;
    case RT_NEW_GAME: return Status == GS_TURN_NONE_WAITING_FOR_START_CONFIRMS ? true : false;
    case RT_MOVE: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_GIVE_UP: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    case RT_SIT_ON: return Status == GS_TURN_NONE_WAITING_FOR_PLAYERS ? true : false;
    case RT_STAND_UP: return whoseTurnFromGameStatus(Status) == NO_TURN ? true : false;
    case RT_PROMOTE_TO:
        if (Status == GS_TURN_WHITE_PROMOTE || Status == GS_TURN_BLACK_PROMOTE) return true;
        else return false;
    case RT_QUEUE_ME: return whoseTurnFromGameStatus(Status) != NO_TURN ? true : false;
    default: return true;
    }
}

bool ChessConditions::isSenderAppropriate(Client& sender, REQUEST_TYPE Type)
{
    if (!_pClientsList->isClientInList(sender)) return false;

    bool bLogged = _pClientsList->isClientLoggedIn(sender);
    bool bSittingOnChair = _pClientsList->isClientAPlayer(sender);
    bool bInQueue = _pClientsList->isClientInQueue(sender);

    switch(Type)
    {
    case RT_NONE:
        qDebug() << "ERROR: ChessConditions::isSenderAppropriate(): Type = RT_NONE";
        return false;
    case RT_NEW_GAME:
    case RT_MOVE:
    case RT_GIVE_UP:
    case RT_STAND_UP:
    case RT_PROMOTE_TO:
        if (bSittingOnChair && !bInQueue) return true;
        else return false;
    case RT_SIT_ON:
    case RT_QUEUE_ME:
        if (bLogged && !bSittingOnChair && !bInQueue) return true;
        else return false;
    case RT_LEAVE_QUEUE:
        if (bLogged && !bSittingOnChair && bInQueue) return true;
        else return false;
    default: return true; //case getTableDataAsJSON
    }
}

bool ChessConditions::isThereAnySpecialConditionBeenMet(Client& sender, clientRequest request)
{
    switch(request.type)
    {
    case RT_NONE:
        qDebug() << "ERROR: ChessConditions::isThereAnySpecialConditionBeenMet(): "
                    "Type = RT_NONE";
        return false;
    case RT_MOVE:
    case RT_PROMOTE_TO:
        if ((_pClientsList->getClientType(sender) == PT_WHITE
             && _pStatus->getWhoseTurn() == WHITE_TURN) ||
                (_pClientsList->getClientType(sender) == PT_BLACK
                 && _pStatus->getWhoseTurn() == BLACK_TURN))
            return true;
        else return false;
    case RT_SIT_ON:
    {
        PLAYER_TYPE PlayerChair = playerTypeFromQStr(request.param);
        if (_pClientsList->isPlayerChairEmpty(PlayerChair, SHOW_ERRORS) &&
                !_pClientsList->isClientAPlayer(sender, SHOW_ERRORS))
            return true;
        else return false;
    }
    case RT_IM: //name == empty || name = actual
        if (sender.name.isEmpty() || _pClientsList->getClientName(sender) == request.param)
            return true;
        else return false;
    case RT_QUEUE_ME:
        if (_pClientsList->isGameTableOccupied())
            return true;
        else return false;
    case RT_LEAVE_QUEUE:
        if (_pClientsList->isClientInQueue(sender))
            return true;
        else return false;
    case RT_CLIENT_LEFT:
        if (_pClientsList->isClientInList(sender))
            return true;
        else return false;
    default: return true;
    }
}
